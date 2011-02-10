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
#ifndef FSCONSOLEDIALOG_H
#define FSCONSOLEDIALOG_H

#include <QDialog>
#include <QVariant.h>
#include <QDateTime.h>
#include "freeswitch.h"

QT_BEGIN_NAMESPACE
class QTextEdit;
class QLineEdit;
QT_END_NAMESPACE

class FSConsoleDialog : public QDialog {
	Q_OBJECT

public:
	FSConsoleDialog(QWidget *parent = 0);
	~FSConsoleDialog();

protected:

public slots:

private slots:
	void onSendCommand();
	void onEventLog(QSharedPointer<switch_log_node_t>,switch_log_level_t);
	void onClear();

signals:
	void closed();

private:
	QTextEdit *_consoleBox;
	QLineEdit *_commandLine;
	void insertColorHtml(QString color, QString text);
};

#endif // FSConsoleDIALOG_H
