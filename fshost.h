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
#ifndef FSHOST_H
#define FSHOST_H

#include <QThread>
#include <QColor>
#include <QHash>
#include <QSharedPointer>
#include <switch.h>
#include "trainer_studio.h"
#include "call.h"
#include "channel.h"

static void eventHandlerCallback(switch_event_t *);
static switch_status_t loggerHandler(const switch_log_node_t *, switch_log_level_t);

class FSHost : public QThread
{
Q_OBJECT
public:
    explicit FSHost(QObject *parent = 0);
    switch_status_t sendCmd(const char *cmd, const char *args, QString *res);
    void generalEventHandler(QSharedPointer<switch_event_t>event);
    void generalLoggerHandler(QSharedPointer<switch_log_node_t>node, switch_log_level_t level);
    void printEventHeaders(QSharedPointer<switch_event_t>event);
    bool isModuleLoaded(QString);
    bool isRunning() { return _running; }
    bool isReady() { return _ready; }
    bool isSofiaReady() { return _sofia_ready; }

protected:
    void run(void);

signals:
    /* Status signals */
    void coreLoadingError(QString);
    void loadingModules(QString, int, QColor);
    void moduleLoaded(QString, QString);
    void ready(void);


    /* Logging signals */
    void eventLog(QSharedPointer<switch_log_node_t>, switch_log_level_t);
    void newEvent(QSharedPointer<switch_event_t>);    

    /* Call signals */
    void incomingCall(QSharedPointer<switch_event_t>event);
    void gatewayStateChange(QString state);
//    void ringing(QSharedPointer<Call>);
//    void answered(QSharedPointer<Call>);
//    void newOutgoingCall(QSharedPointer<Call>);
//    void callFailed(QSharedPointer<Call>);
//    void hungup(QSharedPointer<Call>);

    /* Account signals */
//    void accountStateChange(QSharedPointer<Account>);
//    void newAccount(QSharedPointer<Account>);
//    void delAccount(QSharedPointer<Account>);

private slots:
    /* We need to wait for the gateway deletion before reloading it */
//    void accountReloadSlot(QSharedPointer<Account>);
    void minimalModuleLoaded(QString, QString);

private:
    bool _running;
    bool _ready;
    bool _sofia_ready;
    /* Helper methods */
    void createFolders();

    /*FSM State handlers*/
    /** Channel Related*/
    void eventChannelCreate(QSharedPointer<switch_event_t> event, QString uuid);
    void eventChannelAnswer(QSharedPointer<switch_event_t> event, QString uuid);
    void eventChannelState(QSharedPointer<switch_event_t>event, QString uuid);
    void eventChannelExecute(QSharedPointer<switch_event_t>event, QString uuid);
    void eventChannelExecuteComplete(QSharedPointer<switch_event_t>event, QString uuid);
    void eventChannelOutgoing(QSharedPointer<switch_event_t>event, QString uuid);
    void eventChannelOriginate(QSharedPointer<switch_event_t>event, QString uuid);
    void eventChannelProgress(QSharedPointer<switch_event_t>event, QString uuid);
    void eventChannelProgressMedia(QSharedPointer<switch_event_t>event, QString uuid);
    void eventChannelBridge(QSharedPointer<switch_event_t>event, QString uuid);
    void eventChannelHangup(QSharedPointer<switch_event_t>event, QString uuid);
    void eventChannelUnbridge(QSharedPointer<switch_event_t>event, QString uuid);
    void eventChannelHangupComplete(QSharedPointer<switch_event_t>event, QString uuid);
    void eventChannelDestroy(QSharedPointer<switch_event_t>event, QString uuid);

    /** Others*/
    void eventCodec(QSharedPointer<switch_event_t>event, QString uuid);
    void eventCallUpdate(QSharedPointer<switch_event_t>event, QString uuid);
    void eventRecvInfo(QSharedPointer<switch_event_t>event, QString uuid);

    QSharedPointer<Call> getCurrentActiveCall();

    /* Structures to keep track of things */
    QList<QString> _loadedModules;
    QHash<QString, QSharedPointer<Call> > _active_calls;
    QHash<QString, QSharedPointer<Channel> > _channels;
};

extern FSHost *fshost;

#endif // FSHOST_H
