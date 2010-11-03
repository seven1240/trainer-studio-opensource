/*
 * FreeSWITCH Modular Media Switching Software Library / Soft-Switch Application
 * Copyright (C) 2005-2009, Anthony Minessale II <anthm@freeswitch.org>
 *
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is FreeSWITCH Modular Media Switching Software Library / Soft-Switch Application
 *
 * The Initial Developer of the Original Code is
 * Anthony Minessale II <anthm@freeswitch.org>
 * Portions created by the Initial Developer are Copyright (C)
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Joao Mesquita <jmesquita@freeswitch.org>
 *
 */

#include <QtGui>
#include "freeswitch.h"
#include "isettings.h"

static void eventHandlerCallback(switch_event_t *event);
static switch_status_t loggerHandler(const switch_log_node_t *, switch_log_level_t);

static FreeSwitch *_fs_instance;

FreeSwitch::FreeSwitch(QObject *parent) :
	QThread(parent)
{
	switch_core_setrlimits();
	switch_core_set_globals();

	qRegisterMetaType<QSharedPointer<switch_event_t> >("QSharedPointer<switch_event_t>");
	qRegisterMetaType<QSharedPointer<switch_log_node_t> >("QSharedPointer<switch_log_node_t>");
	qRegisterMetaType<switch_log_level_t>("switch_log_level_t");

	_running = false;
	_ready = false;
	_sofia_ready = false;
	_active_calls = 0;
	_fs_instance = this;
}

void FreeSwitch::createFolders()
{
	/* Create directory structure for softphone with default configs */
	QDir conf_dir = QDir::home();
	if (!conf_dir.exists(DOTDIR))
		conf_dir.mkpath(DOTDIR);
	if (!conf_dir.exists(DOTDIR "/recordings"))
		conf_dir.mkpath(DOTDIR "/recordings");
	if (!conf_dir.exists(DOTDIR "/sounds")) {
		conf_dir.mkpath(DOTDIR "/sounds");
	}
	if (!QFile::exists(QString("%1/" DOTDIR "/conf/freeswitch.xml").arg(conf_dir.absolutePath()))) {
		conf_dir.mkdir(DOTDIR "/conf");
		QFile rootXML(":/conf/freeswitch.xml");
		qDebug() << rootXML.exists(":conf/freeswitch.xml");
		QString dest = QString("%1/" DOTDIR "/conf/freeswitch.xml").arg(conf_dir.absolutePath());
		rootXML.copy(dest);
		QFile destFile(dest);
		destFile.setPermissions(QFile::ReadOwner | QFile::WriteOwner);
	}

	/* Set all directories to the home user directory */
	if (conf_dir.cd(DOTDIR))
	{
		SWITCH_GLOBAL_dirs.conf_dir = (char *) malloc(strlen(QString("%1/conf").arg(conf_dir.absolutePath()).toAscii().constData()) + 1);
		if (!SWITCH_GLOBAL_dirs.conf_dir) {
			emit coreLoadingError("Cannot allocate memory for conf_dir.");
		}
		strcpy(SWITCH_GLOBAL_dirs.conf_dir, QString("%1/conf").arg(conf_dir.absolutePath()).toAscii().constData());

		SWITCH_GLOBAL_dirs.log_dir = (char *) malloc(strlen(QString("%1/log").arg(conf_dir.absolutePath()).toAscii().constData()) + 1);
		if (!SWITCH_GLOBAL_dirs.log_dir) {
			emit coreLoadingError("Cannot allocate memory for log_dir.");
		}
		strcpy(SWITCH_GLOBAL_dirs.log_dir, QString("%1/log").arg(conf_dir.absolutePath()).toAscii().constData());

		SWITCH_GLOBAL_dirs.run_dir = (char *) malloc(strlen(QString("%1/run").arg(conf_dir.absolutePath()).toAscii().constData()) + 1);
		if (!SWITCH_GLOBAL_dirs.run_dir) {
			emit coreLoadingError("Cannot allocate memory for run_dir.");
		}
		strcpy(SWITCH_GLOBAL_dirs.run_dir, QString("%1/run").arg(conf_dir.absolutePath()).toAscii().constData());

		SWITCH_GLOBAL_dirs.db_dir = (char *) malloc(strlen(QString("%1/db").arg(conf_dir.absolutePath()).toAscii().constData()) + 1);
		if (!SWITCH_GLOBAL_dirs.db_dir) {
			emit coreLoadingError("Cannot allocate memory for db_dir.");
		}
		strcpy(SWITCH_GLOBAL_dirs.db_dir, QString("%1/db").arg(conf_dir.absolutePath()).toAscii().constData());

		SWITCH_GLOBAL_dirs.script_dir = (char *) malloc(strlen(QString("%1/script").arg(conf_dir.absolutePath()).toAscii().constData()) + 1);
		if (!SWITCH_GLOBAL_dirs.script_dir) {
			emit coreLoadingError("Cannot allocate memory for script_dir.");
		}
		strcpy(SWITCH_GLOBAL_dirs.script_dir, QString("%1/script").arg(conf_dir.absolutePath()).toAscii().constData());

		SWITCH_GLOBAL_dirs.htdocs_dir = (char *) malloc(strlen(QString("%1/htdocs").arg(conf_dir.absolutePath()).toAscii().constData()) + 1);
		if (!SWITCH_GLOBAL_dirs.htdocs_dir) {
			emit coreLoadingError("Cannot allocate memory for htdocs_dir.");
		}
		strcpy(SWITCH_GLOBAL_dirs.htdocs_dir, QString("%1/htdocs").arg(conf_dir.absolutePath()).toAscii().constData());
	}
}

void FreeSwitch::generalLoggerHandler(QSharedPointer<switch_log_node_t> node, switch_log_level_t level)
{
	emit eventLog(node, level);
}

void FreeSwitch::shutdown()
{
	QString res;
	_running = false;
	sendCmd("fsctl", "shutdown", &res);
}

void FreeSwitch::run(void)
{
	switch_core_flag_t flags = SCF_USE_SQL | SCF_USE_AUTO_NAT;
	const char *err = NULL;
	switch_bool_t console = SWITCH_FALSE;
	switch_status_t destroy_status;

	createFolders();

	ISettings *settings = new ISettings();
	settings->resetGateway();
	settings->saveToFile();
	delete settings;

	_running = true;

	/* If you need to override configuration directories, you need to change them in the SWITCH_GLOBAL_dirs global structure */
	qDebug() << "FS: initializing core...";

	/* Initialize the core and load modules, that will startup FS completely */
	if (switch_core_init(flags, console, &err) != SWITCH_STATUS_SUCCESS) {
		fprintf(stderr, "Failed to initialize FreeSWITCH's core: %s\n", err);
		emit coreLoadingError(err);
	}

	qDebug() << "FS: binding callback...";

	if (switch_event_bind("TS", SWITCH_EVENT_ALL, SWITCH_EVENT_SUBCLASS_ANY, eventHandlerCallback, NULL) != SWITCH_STATUS_SUCCESS) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "Couldn't bind!\n");
	}

	qDebug() << "FS: loading modules...";

	emit loadingModules("Loading modules...", Qt::AlignRight|Qt::AlignBottom, Qt::blue);
	if (switch_core_init_and_modload(flags, console, &err) != SWITCH_STATUS_SUCCESS) {
		fprintf(stderr, "Failed to initialize FreeSWITCH's core: %s\n", err);
		emit coreLoadingError(err);
	}

	emit allModulesLoaded();

	qDebug() << "FS: binding logger...";

	switch_log_bind_logger(loggerHandler, SWITCH_LOG_DEBUG, SWITCH_FALSE);
	_ready = true;
	emit ready();

	/* Go into the runtime loop. If the argument is true, this basically sets runtime.running = 1 and loops while that is set
	 * If its false, it initializes the libedit for the console, then does the same thing */
	if (_running) {
		qDebug() << "FS: entering runtime loop...";
		switch_core_runtime_loop(!console);
	}
	else {
		qDebug() << "FS: shutdown before startup...";
	}

	switch_event_unbind_callback(eventHandlerCallback);
	destroy_status = switch_core_destroy();
	if (destroy_status == SWITCH_STATUS_SUCCESS)
	{
		qDebug() << "FS: we have properly shutdown the core.";
	}
	else
	{
		qDebug() << "FS: errors during FS shutdown.";
	}
	_running = false;
}

void FreeSwitch::generalEventHandler(switch_event_t *switchEvent)
{
	QSharedPointer<switch_event_t> event(switchEvent);
	QString uuid = switch_event_get_header_nil(event.data(), "Unique-ID");

	// case SWITCH_EVENT_CUSTOM: break;
	// case SWITCH_EVENT_CHANNEL_CREATE: break;
	// case SWITCH_EVENT_CHANNEL_BRIDGE: break;
	// case SWITCH_EVENT_CHANNEL_UNBRIDGE: break;
	// case SWITCH_EVENT_MODULE_LOAD: break;
	switch (event.data()->event_id) {
	case SWITCH_EVENT_CLONE: break;
	case SWITCH_EVENT_CHANNEL_DESTROY: break;
	case SWITCH_EVENT_CHANNEL_STATE: break;
	case SWITCH_EVENT_CHANNEL_CALLSTATE: break;
	case SWITCH_EVENT_CHANNEL_ANSWER: break;
	{
		qDebug() << "Channel Answer" << uuid;
		emit callAnswered(uuid);
		break;
	}
	case SWITCH_EVENT_CHANNEL_HANGUP: break;
	case SWITCH_EVENT_CHANNEL_HANGUP_COMPLETE:
	{
		qDebug() << "Channel End" << uuid;
		emit callEnded(uuid);
		break;
	}
	case SWITCH_EVENT_CHANNEL_EXECUTE: break;
	case SWITCH_EVENT_CHANNEL_EXECUTE_COMPLETE: break;
	case SWITCH_EVENT_CHANNEL_HOLD: break;
	case SWITCH_EVENT_CHANNEL_UNHOLD: break;
	case SWITCH_EVENT_CHANNEL_PROGRESS: break;
	case SWITCH_EVENT_CHANNEL_PROGRESS_MEDIA: break;
	case SWITCH_EVENT_CHANNEL_OUTGOING: break;
	case SWITCH_EVENT_CHANNEL_PARK: break;
	case SWITCH_EVENT_CHANNEL_UNPARK: break;
	case SWITCH_EVENT_CHANNEL_APPLICATION: break;
	case SWITCH_EVENT_CHANNEL_ORIGINATE: break;
	case SWITCH_EVENT_CHANNEL_UUID: break;
	case SWITCH_EVENT_API: break;
	case SWITCH_EVENT_LOG: break;
	case SWITCH_EVENT_INBOUND_CHAN: break;
	case SWITCH_EVENT_OUTBOUND_CHAN: break;
	case SWITCH_EVENT_STARTUP: break;
	case SWITCH_EVENT_SHUTDOWN: break;
	case SWITCH_EVENT_PUBLISH: break;
	case SWITCH_EVENT_UNPUBLISH: break;
	case SWITCH_EVENT_TALK: break;
	case SWITCH_EVENT_NOTALK: break;
	case SWITCH_EVENT_SESSION_CRASH: break;
	case SWITCH_EVENT_MODULE_UNLOAD: break;
	case SWITCH_EVENT_DTMF: break;
	case SWITCH_EVENT_MESSAGE: break;
	case SWITCH_EVENT_PRESENCE_IN: break;
	case SWITCH_EVENT_NOTIFY_IN: break;
	case SWITCH_EVENT_PRESENCE_OUT: break;
	case SWITCH_EVENT_PRESENCE_PROBE: break;
	case SWITCH_EVENT_MESSAGE_WAITING: break;
	case SWITCH_EVENT_MESSAGE_QUERY: break;
	case SWITCH_EVENT_ROSTER: break;
	case SWITCH_EVENT_CODEC: break;
	case SWITCH_EVENT_BACKGROUND_JOB: break;
	case SWITCH_EVENT_DETECTED_SPEECH: break;
	case SWITCH_EVENT_DETECTED_TONE: break;
	case SWITCH_EVENT_PRIVATE_COMMAND: break;
	case SWITCH_EVENT_HEARTBEAT: break;
	case SWITCH_EVENT_TRAP: break;
	case SWITCH_EVENT_ADD_SCHEDULE: break;
	case SWITCH_EVENT_DEL_SCHEDULE: break;
	case SWITCH_EVENT_EXE_SCHEDULE: break;
	case SWITCH_EVENT_RE_SCHEDULE: break;
	case SWITCH_EVENT_RELOADXML: break;
	case SWITCH_EVENT_NOTIFY: break;
	case SWITCH_EVENT_SEND_MESSAGE: break;
	case SWITCH_EVENT_RECV_MESSAGE: break;
	case SWITCH_EVENT_REQUEST_PARAMS: break;
	case SWITCH_EVENT_CHANNEL_DATA: break;
	case SWITCH_EVENT_GENERAL: break;
	case SWITCH_EVENT_COMMAND: break;
	case SWITCH_EVENT_SESSION_HEARTBEAT: break;
	case SWITCH_EVENT_CLIENT_DISCONNECTED: break;
	case SWITCH_EVENT_SERVER_DISCONNECTED: break;
	case SWITCH_EVENT_SEND_INFO: break;
	case SWITCH_EVENT_RECV_INFO: break;
	case SWITCH_EVENT_RECV_RTCP_MESSAGE: break;
	case SWITCH_EVENT_CALL_SECURE: break;
	case SWITCH_EVENT_NAT: break;
	case SWITCH_EVENT_RECORD_START: break;
	case SWITCH_EVENT_RECORD_STOP: break;
	case SWITCH_EVENT_CALL_UPDATE: break;
	case SWITCH_EVENT_FAILURE: break;
	case SWITCH_EVENT_SOCKET_DATA: break;
	case SWITCH_EVENT_MEDIA_BUG_START: break;
	case SWITCH_EVENT_MEDIA_BUG_STOP: break;
	case SWITCH_EVENT_ALL: break;
	case SWITCH_EVENT_CHANNEL_CREATE:
	{
		if (_active_calls > 0) {
			QString res;
			sendCmd("uuid_kill", (uuid + " USER_BUSY").toAscii(), &res);
			qDebug() << "hangup " << res;
		}
		break;
	}
	case SWITCH_EVENT_CHANNEL_BRIDGE:/*27A*/
	{
		_active_calls++;
		break;
	}
	case SWITCH_EVENT_CHANNEL_UNBRIDGE:/*34A*/
	{
		if (--_active_calls < 0)
			_active_calls = 0;
		break;
	}
	case SWITCH_EVENT_CUSTOM:/*5A*/
	{
		if (strcmp(event.data()->subclass_name, "portaudio::ringing") == 0) {
			emit callIncoming(uuid);
		}
		else if (strcmp(event.data()->subclass_name, "sofia::gateway_state") == 0) {
			QString gw = switch_event_get_header_nil(event.data(), "Gateway");
			QString state = switch_event_get_header_nil(event.data(), "State");
			qDebug() << "Gateway State:" << gw << " " << state;
			emit gatewayStateChange(gw, state);
			emit gatewayReady(gw, state == "REGED");
		}
		else if (strcmp(event.data()->subclass_name, "sofia::gateway_add") == 0)
		{
			QString gw = switch_event_get_header_nil(event.data(), "Gateway");
			qDebug() << "Gateway Added:" << gw;
			emit gatewayAdded(gw);
			/*
			Account * accPtr = new Account(gw);
			QSharedPointer<Account> acc = QSharedPointer<Account>(accPtr);
			acc.data()->setState(FSCOMM_GW_STATE_NOAVAIL);
			_accounts.insert(gw, acc);
			emit newAccount(acc);
			*/
		}
		else if (strcmp(event.data()->subclass_name, "sofia::gateway_delete") == 0)
		{
			QString gw = switch_event_get_header_nil(event.data(), "Gateway");
			qDebug() << "Gateway Deleted:" << gw;
			emit gatewayDeleted(gw);
			/*
			QSharedPointer<Account> acc = _accounts.take(gw);
			if (!acc.isNull())
			emit delAccount(acc);
			*/
		}
		else
		{
		}
		break;
	}
	case SWITCH_EVENT_MODULE_LOAD:
	{
		QString modType = switch_event_get_header_nil(event.data(), "type");
		QString modKey = switch_event_get_header_nil(event.data(), "key");
		QString modName = switch_event_get_header_nil(event.data(), "name");
		if (modKey == "mod_sofia") {
			_sofia_ready = true;
			emit sofiaReady();
		}
		emit moduleLoaded(modType, modKey, modName);
		break;
	}
	default:
		break;
	}

	emit newEvent(event);
}

void FreeSwitch::minimalModuleLoaded(QString modType, QString modKey, QString /*modName*/)
{
	if (modType == "endpoint")
	{
		_loadedModules.append(modKey);
	}
}

switch_status_t FreeSwitch::sendCmd(const char *cmd, const char *args, QString *res)
{
	switch_status_t status = SWITCH_STATUS_FALSE;
	switch_stream_handle_t stream;
	SWITCH_STANDARD_STREAM(stream);
	status = switch_api_execute(cmd, args, NULL, &stream);
	*res = switch_str_nil((char *) stream.data);
	switch_safe_free(stream.data);

	return status;
}

QSharedPointer<Call> FreeSwitch::getCurrentActiveCall()
{
	foreach (QSharedPointer<Call> call, _activeCalls.values())
	{
		if (call.data()->isActive())
			return call;
	}
	return QSharedPointer<Call>();
}

void FreeSwitch::printEventHeaders(QSharedPointer<switch_event_t>event)
{
	switch_event_header_t *hp;
	qDebug() << QString("Received event: %1(%2)").arg(switch_event_name(event.data()->event_id), switch_event_get_header_nil(event.data(), "Event-Subclass"));
	for (hp = event.data()->headers; hp; hp = hp->next) {
		qDebug() << hp->name << "=" << hp->value;
	}
	qDebug() << "\n\n";
}

QString FreeSwitch::call(QString callee)
{
	QString res;
	sendCmd("pa", ("call " + callee).toAscii(), &res);
	qDebug() << "Call: " << res.trimmed();
	QStringList sl = res.split(":");
	if (sl.count() == 3 && sl.at(0) == "SUCCESS") {
		emit callOutgoing(sl.at(2).trimmed());
	}
	return res;
}

void FreeSwitch::reload()
{
	QString res;
	sendCmd("sofia", "profile softphone rescan reloadxml", &res);
}

void FreeSwitch::setupGateway(QString username, QString password, QString realm, bool tcp)
{
	ISettings *settings = new ISettings(this);
	qDebug() << "FS: setting gateway: " << username << password << realm;
	QVariantMap gw;
	gw.insert("username", username);
	gw.insert("password", password);
	gw.insert("realm", realm);
	if (tcp) {
		gw.insert("register-transport", "tcp");
	}
	settings->writeGateway(gw);
	settings->saveToFile();
	delete settings;
	reload();
}

switch_status_t FreeSwitch::mute()
{
	QString res;
	return sendCmd("pa", "flags off mouth", &res);
}

switch_status_t FreeSwitch::unmute()
{
	QString res;
	return sendCmd("pa", "flags on mouth", &res);
}

switch_status_t FreeSwitch::hold(QString uuid)
{
	QString res;
	return sendCmd("uuid_hold", uuid.toAscii(), &res);
}

switch_status_t FreeSwitch::unhold(QString uuid)
{
	QString res;
	return sendCmd("uuid_hold", ("off " + uuid).toAscii(), &res);
}

void FreeSwitch::hangup(bool all)
{
	QString res;
	if (all) {
		sendCmd("hupall", "", &res);
	}
	else {
		sendCmd("pa", "hangup", &res);
	}
}

switch_status_t FreeSwitch::recordStart(QString uuid, QString filename)
{
	QString res;
	return sendCmd("uuid_record", QString("%1 start %2").arg(uuid, filename).toAscii().data(), &res);
}

switch_status_t FreeSwitch::recordStop(QString uuid, QString filename)
{
	QString res;
	return sendCmd("uuid_record", QString("%1 stop %2").arg(uuid, filename).toAscii().data(), &res);
}

void FreeSwitch::answer()
{
	QString res;
	sendCmd("pa", "answer", &res);
}

switch_status_t FreeSwitch::portAudioDtmf(char chr)
{
	QString res;
	QString params = QString("dtmf %1").arg(chr);
	return sendCmd("pa", params.toAscii(), &res);
}

QString FreeSwitch::portAudioRescan()
{
	QString devices;
	sendCmd("pa", "rescan", &devices);
	return devices;
}

void FreeSwitch::portAudioLoop()
{
	QString res;
	sendCmd("pa", "looptest", &res);
}

void FreeSwitch::portAudioPlay(QString target)
{
	QString res;
	sendCmd("pa", QString("play %1").arg(target).toAscii(), &res);
}

QString FreeSwitch::portAudioDevices()
{
	QString devices;
	switch_status_t status = sendCmd("pa", "devlist", &devices);
	if (SWITCH_STATUS_SUCCESS != status) {
		return "";
	}
	return devices;
}

void FreeSwitch::portAudioInDevice(int index)
{
	QString res;
	sendCmd("pa", QString("indev #%1").arg(index).toAscii(), &res);
}

void FreeSwitch::portAudioOutDevice(int index)
{
	QString res;
	sendCmd("pa", QString("outdev #%1").arg(index).toAscii(), &res);
}

void FreeSwitch::portAudioRingDevice(int index)
{
	QString res;
	sendCmd("pa", QString("ringdev #%1").arg(index).toAscii(), &res);
}

static void eventHandlerCallback(switch_event_t *event)
{
	switch_event_t *clone = NULL;
	if (switch_event_dup(&clone, event) == SWITCH_STATUS_SUCCESS) {
		_fs_instance->generalEventHandler(clone);
	}
}

static switch_status_t loggerHandler(const switch_log_node_t *node, switch_log_level_t level)
{
	switch_log_node_t *clone = switch_log_node_dup(node);
	QSharedPointer<switch_log_node_t> l(clone);
	_fs_instance->generalLoggerHandler(l, level);
	return SWITCH_STATUS_SUCCESS;
}

