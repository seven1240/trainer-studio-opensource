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
 */
#include <QtGui>
#include <QTextEdit>
#include <QLineEdit>
#include "fsconsole_dialog.h"
#include "application_controller.h"
#include "utils.h"

FSConsoleDialog::FSConsoleDialog(QWidget *parent) :
	QDialog(parent)
{
	_consoleBox = new QTextEdit();
	_consoleBox->setReadOnly(true);

	_commandLine = new QLineEdit();

//	QHBoxLayout *toolBar = new QHBoxLayout();
	QPushButton *clear = new QPushButton("Clear");
	clear->setAutoDefault(false);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(clear);
	layout->addWidget(_consoleBox);
	layout->addWidget(_commandLine);
	setLayout(layout);
	_commandLine->setFocus();

	this->resize(640, 480);
	Utils::centerWindowOnDesktop(this);

	connect(clear, SIGNAL(clicked()), this, SLOT(onClear()));
	connect(_commandLine, SIGNAL(returnPressed()), this, SLOT(onSendCommand()));
	connect(ApplicationController::fs(),
			SIGNAL(eventLog(QSharedPointer<switch_log_node_t>,switch_log_level_t)),
			this,
			SLOT(onEventLog(QSharedPointer<switch_log_node_t>,switch_log_level_t)));
}

FSConsoleDialog::~FSConsoleDialog()
{
	delete _consoleBox;
	delete _commandLine;
}

void FSConsoleDialog::onClear()
{
	_consoleBox->clear();
}

void FSConsoleDialog::onSendCommand()
{
	QString cmd, args, res;
	QStringList cmdList = _commandLine->text().split(" ");

	if (cmdList.isEmpty()) return;

	_consoleBox->append(">>> " + _commandLine->text());

	cmd = cmdList[0];
	cmdList.removeFirst();
	args = cmdList.join(" ");
	ApplicationController::fs()->command(cmd.toAscii(), args.toAscii(), &res);
	_consoleBox->append(">>> " + res);

	QScrollBar *sb = _consoleBox->verticalScrollBar();
	sb->setValue(sb->maximum());
}

void FSConsoleDialog::onEventLog(QSharedPointer<switch_log_node_t> node,switch_log_level_t level)
{
	QString color = "";

	switch (level) {
	case SWITCH_LOG_DEBUG:
			color = "#ffaa56"; break;
	case SWITCH_LOG_ALERT:
	case SWITCH_LOG_CRIT:
	case SWITCH_LOG_ERROR:
			color = "#FF0000"; break;
	case SWITCH_LOG_WARNING:
			color = "#ff56aa"; break;
	case SWITCH_LOG_NOTICE:
			color = "#0000ff"; break;
	case SWITCH_LOG_INFO:
			color = "#00ffff"; break;
	default:
			color = "#000000";
	}
	insertColorHtml(color, QString(node.data()->data).trimmed().replace("\n", "<br>"));

	QScrollBar *sb = _consoleBox->verticalScrollBar();
	sb->setValue(sb->maximum());
}

void FSConsoleDialog::insertColorHtml(QString color, QString text)
{
	_consoleBox->insertHtml("<span style='color:" + color + "'>" +
		text + "</span><br>\n");
}
