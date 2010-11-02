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
#include "application_controller.h"
#include "call.h"
#include "fs_host.h"

Call::Call()
{
	_answeredEpoch = 0;
}

switch_status_t Call::toggleHold(bool hold)
{
	if (_state != FSCOMM_CALL_STATE_ANSWERED)
		return SWITCH_STATUS_FALSE;

	if (hold)
	{
		return ApplicationController::fs()->hold(_channel.data()->getUuid());
	}
	else
	{
		return ApplicationController::fs()->unhold(_channel.data()->getUuid());
	}
}

switch_status_t Call::toggleRecord(bool startRecord)
{
	QDir conf_dir = QDir::home();

	if (startRecord)
	{
		_recording_filename = QString("%1/.fscomm/recordings/%2_%3.wav").arg(conf_dir.absolutePath(), QDateTime::currentDateTime().toString("yyyyMMddhhmmss"), getCidNumber());
		return ApplicationController::fs()->recordStart(getUuid(), _recording_filename);
	}
	else
	{
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_DEBUG, "Stopping call recording on call [%s]\n", getUuid().toAscii().data());
		return ApplicationController::fs()->recordStop(getUuid(), _recording_filename);
	}
}

void Call::sendDTMF(QString digit)
{
	if (ApplicationController::fs()->portAudioDtmf(digit.toAscii()[0]) == SWITCH_STATUS_FALSE) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "Could not send DTMF digit %s on call[%s]", digit.toAscii().data(), getUuid().toAscii().data());
		QMessageBox::critical(0, QWidget::tr("DTMF Error"), QWidget::tr("There was an error sending DTMF, please report this bug."), QMessageBox::Ok);
	}
}

QTime Call::getCurrentStateTime()
{
	qulonglong time = 0;

	if (_state == FSCOMM_CALL_STATE_ANSWERED)
	{
		time = _answeredEpoch;
	}
	else if(_state == FSCOMM_CALL_STATE_RINGING)
	{
		if (_direction == FSCOMM_CALL_DIRECTION_INBOUND)
		{
			time = _channel.data()->getCreatedEpoch();
		}
		else
			_otherLegChannel.data()->getProgressEpoch() == 0 ? time = _otherLegChannel.data()->getProgressMediaEpoch() : time = _otherLegChannel.data()->getProgressEpoch();
	}

	int now = QDateTime::fromTime_t(time).secsTo(QDateTime::currentDateTime());
	return QTime::fromString(QString::number(now), "s");
}
