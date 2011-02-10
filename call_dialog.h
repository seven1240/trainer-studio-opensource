/*
 * Eleutian Trainer Studio Application Opensource Edition, a VoIP client with Webkit and Flash support.
 * Copyright (C) 2011, Eleutian Technology, LLC <http://www.eleutian.com>
 *
 * Version: Apache License 2.0
 *
 * The content of this file is licensed under the Apache License, Version 2.0.  (the "License").
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
#ifndef CALL_DIALOG_H
#define CALL_DIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QTextEdit;
class QLineEdit;
class QPushButton;
class QLabel;
QT_END_NAMESPACE

class DialPadWidget;

class CallDialog : public QDialog {
	Q_OBJECT

public:
	CallDialog(QWidget *parent = 0);
	~CallDialog();

protected:
	void keyPressEvent(QKeyEvent *event);
	void closeEvent(QCloseEvent *event);
	void showEvent(QShowEvent *e);

signals:
	void closed();

private:
	DialPadWidget *_dialPadWidget;
	QTextEdit *_display;
	QLineEdit *_number;
	QPushButton *_call;
	QPushButton *_hold;
	QPushButton *_answer;
	QPushButton *_hangup;
	QPushButton *_conf;
	QPushButton *_n800;
	QPushButton *_echo;
	QLabel *_sipStatusLabel;

private slots:
	void dialed(QString dtmf);
	void onCall();
	void onHold();
	void onAnswer();
	void onHangup();
	void onCallConf();
	void onCall800();
	void onCallEcho();

	void onGatewayStateChange(QString gwName, QString gwState);
	void onCallIncoming(QString uuid, QString cidNumber, QString cidName);
	void onCallAnswered(QString uuid, QString cidNumber, QString cidName);
	void onCallEnded(QString uuid, QString cidNumber, QString cidName);

};

#endif // CALL_DIALOG_H
