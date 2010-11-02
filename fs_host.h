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

class FSHost : public QThread
{
  Q_OBJECT

public:
  FSHost(QObject *parent = 0);
  bool isRunning() { return _running; }
  bool isReady() { return _ready; }
  bool isSofiaReady() { return _sofia_ready; }
  bool isBusy() { return _active_calls > 0; }

protected:
  void run(void);

public:
  QString call(QString callee);
  void reload();
  switch_status_t mute();
  switch_status_t unmute();
  switch_status_t hold(QString uuid);
  switch_status_t unhold(QString uuid);
  void hangup(bool all);
  switch_status_t recordStart(QString uuid, QString filename);
  switch_status_t recordStop(QString uuid, QString filename);
  void answer();

  switch_status_t portAudioDtmf(char chr);
  QString portAudioRescan();
  void portAudioLoop();
  void portAudioPlay(QString target);
  QString portAudioDevices();
  void portAudioInDevice(int index);
  void portAudioOutDevice(int index);
  void portAudioRingDevice(int index);

public:
  void generalEventHandler(switch_event_t *event);
  void generalLoggerHandler(QSharedPointer<switch_log_node_t> node, switch_log_level_t level);
  void printEventHeaders(QSharedPointer<switch_event_t> event);
  void createFolders();

signals:
  /* Status signals */
  void coreLoadingError(QString);
  void loadingModules(QString, int, QColor);
  void moduleLoaded(QString, QString, QString);
  void ready(void);

  /* Logging signals */
  void eventLog(QSharedPointer<switch_log_node_t>, switch_log_level_t);
  void newEvent(QSharedPointer<switch_event_t>);    

  /* Call signals */
  void incomingCall(QSharedPointer<switch_event_t>event);
  void gatewayStateChange(QString state);

private slots:
  void minimalModuleLoaded(QString, QString, QString);

private:
  switch_status_t sendCmd(const char *cmd, const char *args, QString *res);
  QSharedPointer<Call> getCurrentActiveCall();
  QList<QString> _loadedModules;
  QHash<QString, QSharedPointer<Call> > _activeCalls;
  QHash<QString, QSharedPointer<Channel> > _activeChannels;
  bool _running;
  bool _ready;
  bool _sofia_ready;
  int _active_calls;
};

extern FSHost *fs;

#endif // FSHOST_H
