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
#ifndef ECHO_TEST_DIALOG_H
#define ECHO_TEST_DIALOG_H

#include <QDialog>
#include "freeswitch.h"

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
QT_END_NAMESPACE

class EchoTestDialog : public QDialog {
	Q_OBJECT
public:
	EchoTestDialog(QWidget *parent = 0);
	~EchoTestDialog();

protected:
	void changeEvent(QEvent *e);
	void showEvent(QShowEvent *e);
	void closeEvent(QCloseEvent *e);

signals:
	void closed();

private:
	QPushButton *_begin;
	QPushButton *_skip;
	QPushButton *_finish;
	QLabel *_progress;

private:
	void progress(QString string);
	bool didTheyHearThemselves();

private slots:
	void onSkipClicked();
	void onBeginClicked();
	void onFinishClicked();
	void onNewEvent(QSharedPointer<switch_event_t>);
};

#endif // ECHO_TEST_DIALOG_H
