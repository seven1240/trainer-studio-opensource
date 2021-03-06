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
#include <QDebug.h>
#include <QMessageBox>
#include <QApplication>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QTextEdit>
#include <QtGui/QKeyEvent>
#include <QtGui/QInputDialog>
#include "application_controller.h"
#include "freeswitch.h"
#include "call_dialog.h"
#include "dial_pad_widget.h"
#include "utils.h"
#include "user.h"
#include "trainer_studio.h"

CallDialog::CallDialog(QWidget *parent) : QDialog(parent)
{
	_display = new QTextEdit();
	_number = new QLineEdit();
	_call = new QPushButton("Call");
	_hold = new QPushButton("Hold");
	_answer = new QPushButton("Answer");
	_hangup = new QPushButton("Hangup");
	_conf = new QPushButton("Conf 20");
	_n800 = new QPushButton("Conf 30");
	_conf->setToolTip("Call Conference Room 20");
	_n800->setToolTip("Call Conference Room 30");
	_echo = new QPushButton("Echo Test");
	_sipStatusLabel = new QLabel("SIP: None");

	_display->setReadOnly(true);

	_dialPadWidget = new DialPadWidget();

	QGridLayout *layout = new QGridLayout();
	layout->setHorizontalSpacing(-1);
	layout->setVerticalSpacing(-1);
	layout->addWidget(_display, 0, 0, 1, 3);
	layout->addWidget(_number, 1, 0, 1, 2);
	layout->addWidget(_call, 1, 2, 1, 1);
	layout->addWidget(_hold, 2, 1, 1, 1);
	layout->addWidget(_answer, 2, 0, 1, 1);
	layout->addWidget(_hangup, 2, 2, 1, 1);
	layout->addWidget(_conf, 3, 0, 1, 1);
	layout->addWidget(_n800, 3, 1, 1, 1);
	layout->addWidget(_echo, 3, 2, 1, 1);
	layout->addWidget(_dialPadWidget, 4, 0, 1, 3);
	layout->addWidget(_sipStatusLabel, 5, 0, 1, 3);
	setLayout(layout);

	setWindowTitle("Call");
	setFixedSize(300, 380);

	connect(_number, SIGNAL(returnPressed()), this, SLOT(onCall()));
	connect(_dialPadWidget, SIGNAL(dialed(QString)), this, SLOT(dialed(QString)));
	connect(_call, SIGNAL(clicked()), this, SLOT(onCall()));
	connect(_hold, SIGNAL(clicked()), this, SLOT(onHold()));
	connect(_answer, SIGNAL(clicked()), this, SLOT(onAnswer()));
	connect(_hangup, SIGNAL(clicked()), this, SLOT(onHangup()));
	connect(_conf, SIGNAL(clicked()), this, SLOT(onCallConf()));
	connect(_n800, SIGNAL(clicked()), this, SLOT(onCall800()));
	connect(_echo, SIGNAL(clicked()), this, SLOT(onCallEcho()));

	connect(ApplicationController::fs(), SIGNAL(gatewayStateChange(QString, QString)),
		this, SLOT(onGatewayStateChange(QString, QString)));
	connect(ApplicationController::fs(), SIGNAL(newIncomingCall(QString, QString, QString)),
		this, SLOT(onCallIncoming(QString, QString, QString)));
	connect(ApplicationController::fs(), SIGNAL(callAnswered(QString, QString, QString)),
		this, SLOT(onCallAnswered(QString, QString, QString)));
	connect(ApplicationController::fs(), SIGNAL(callEnded(QString, QString, QString)),
		this, SLOT(onCallEnded(QString, QString, QString)));
}

CallDialog::~CallDialog()
{
}

void CallDialog::keyPressEvent(QKeyEvent * /*event*/)
{
}

void CallDialog::showEvent(QShowEvent * /*event*/)
{
	_call->setEnabled(true);
}

void CallDialog::closeEvent(QCloseEvent * /*event*/)
{
	emit closed();
}

void CallDialog::onCall()
{
	QString number = _number->text();
	QString cidName = ApplicationController::user()->getLogin();
	QString cidNumber = ApplicationController::user()->getVoipUsername();
	if (!number.isEmpty()) {
		_display->append(QString("Calling %2").arg(number));
		ApplicationController::fs()->call(number, cidName, cidNumber);
	}
	_call->setEnabled(false);
}

void CallDialog::onHold()
{
}

void CallDialog::onAnswer()
{
	ApplicationController::fs()->answer();
}

void CallDialog::onHangup()
{
	ApplicationController::fs()->hangup(true);
	_call->setEnabled(true);
}

void CallDialog::onCallConf()
{
	_number->setText("conf_20");
}

void CallDialog::onCall800()
{
	_number->setText("conf_30");
}

void CallDialog::onCallEcho()
{
	_number->setText("echo");
}

void CallDialog::dialed(QString dtmf)
{
	if (dtmf.length() > 0) {
		ApplicationController::fs()->portAudioDtmf(dtmf.toAscii()[0]);
		_display->setText(_display->toPlainText() + dtmf);
	}
}

void CallDialog::onGatewayStateChange(QString /*gwName*/, QString gwState)
{
	if (gwState == "TRYING" || gwState == "REGISTER") {
		// do nothing
	}
	else { //REGED UNREGED UNREGISTER FAILED FAIL_WAIT EXPIRED NOREG NOAVAIL
	}
	_sipStatusLabel->setText(QString("SIP State: %1").arg(gwState));
}

void CallDialog::onCallIncoming(QString /*uuid*/, QString cidNumber, QString cidName)
{
	_display->append(QString("RingFrom: %2").arg(
		Utils::formatCallerID(cidName, cidNumber)));
}

void CallDialog::onCallAnswered(QString /*uuid*/, QString cidNumber, QString cidName)
{
	_display->append(QString("Answered: %2").arg(
		Utils::formatCallerID(cidName, cidNumber)));
}

void CallDialog::onCallEnded(QString /*uuid*/, QString cidNumber, QString cidName)
{
	_display->append(QString("Ended: %2").arg(
		Utils::formatCallerID(cidName, cidNumber)));
	_call->setEnabled(true);
}

