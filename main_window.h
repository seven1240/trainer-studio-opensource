/*
 * Eleutian Trainer Studio Application Opensource Edition, a VoIP client with Webkit and Flash support.
 * Copyright (C) 2011, Eleutian Technology, LLC <http://www.eleutian.com>
 *
 * Version: Apache License 2.0
 *
 * The content of this file is licensed under the Apache License, Version 2.0. (the "License").
 * You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.apache.org/licenses/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Eleutian Trainer Studio Application Opensource Edition.
 *
 * The Initial Developer of the Original Code is
 * Seven Du <seven@eleutian.com>
 * Portions created by the Initial Developer are Copyright (C)
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Jadob Lewallen <jacob@eleutian.com>
 */
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QToolButton>
#include <QKeyEvent>
#include "settings_dialog.h"
#include "trainer_studio.h"
#include "freeswitch.h"
#include "user.h"

QT_BEGIN_NAMESPACE
class QSystemTrayIcon;
QT_END_NAMESPACE

class MainWindow : public QWidget {
	Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	void showEvent(QShowEvent *e);
	void changeEvent(QEvent *e);
	QLayout *createBody();

private:
	QPushButton *_state;
	QLabel *_sipStatusLabel;
	QToolButton *_sipReg;
	bool _sipStateReady;
	bool _isPaused;
	QString _activeUUID;
	QSystemTrayIcon *_systemTray;
	QTimer *_timer;
	void parseCallResult(QString res);

signals:
	void call();
	void testEcho();
	void testFlash();

private slots:
	void on_Hangup_clicked();
	void on_Call_clicked();
	void on_Echo_clicked();
	void on_State_clicked();
	void on_Flash_clicked();
	void on_About_clicked();
	void on_Logout_clicked();
	void on_Settings_clicked();
	void on_SipReg_clicked();
	void onPaused(bool);
	void onForcedPause(QString reason);
	void onGatewayStateChange(QString name, QString state);
	void onReservedForInteraction(QVariantMap);
	void onTimer();
};

#endif // MAIN_WINDOW_H
