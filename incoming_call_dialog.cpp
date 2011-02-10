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
 * Jadob Lewallen <jacob@eleutian.com>
 * Portions created by the Initial Developer are Copyright (C)
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Seven Du <seven@eleutian.com>
 */
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QShortcut>

#include "application_controller.h"
#include "incoming_call_dialog.h"
#include "freeswitch.h"
#include "utils.h"

IncomingCallDialog::IncomingCallDialog(QWidget *parent) :
	QDialog(parent)
{
	QGridLayout *layout = new QGridLayout();
	_answer = new QPushButton("&Answer (Ctrl-A)");
	_reject = new QPushButton("Hangup");
	_status = new QLabel();
	layout->addWidget(_status, 0, 0, 1, 4, Qt::AlignHCenter);
	layout->addWidget(_reject, 1, 0, 1, 1, Qt::AlignLeft | Qt::AlignBottom);
	layout->addWidget(_answer, 1, 1, 1, 3, Qt::AlignRight | Qt::AlignBottom);
	setLayout(layout);

	QShortcut *ctrlA = new QShortcut(QKeySequence(tr("Ctrl+A")), this);
	QShortcut *commandA = new QShortcut(QKeySequence(tr("Cmd+A")), this);
	_answer->setFixedWidth(200);

	setFixedSize(320, 140);
	setWindowTitle("Incoming Call");
	setWindowFlags(Qt::WindowStaysOnTopHint);

	Utils::centerWindowOnDesktop(this);

	connect(ctrlA, SIGNAL(activated()), this, SLOT(onAnswerClicked()));
	connect(commandA, SIGNAL(activated()), this, SLOT(onAnswerClicked()));
	connect(_answer, SIGNAL(clicked()), this, SLOT(onAnswerClicked()));
	connect(_reject, SIGNAL(clicked()), this, SLOT(onRejectClicked()));
	connect(ApplicationController::fs(), SIGNAL(callIncoming(QString,QString,QString)), this, SLOT(onIncomingCall(QString,QString,QString)));
}

IncomingCallDialog::~IncomingCallDialog()
{
}

void IncomingCallDialog::showEvent(QShowEvent * /*e*/)
{
	_answer->setFocus();
}

void IncomingCallDialog::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		// ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void IncomingCallDialog::onIncomingCall(QString /*uuid*/, QString cidNumber, QString cidName)
{
	_callerName = cidName;
	_callerNumber = cidNumber;
	_status->setText(QString("\"%1\" <%2>").arg(_callerName).arg(_callerNumber));
	_status->repaint();
}

void IncomingCallDialog::onAnswerClicked()
{
	ApplicationController::fs()->answer();
	accept();
}

void IncomingCallDialog::onRejectClicked()
{
	ApplicationController::fs()->hangup();
	reject();
}
