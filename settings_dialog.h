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
#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>

#define TAB_GENERAL 1
#define TAB_AUDIO 1 << 1
#define TAB_OTHER 1 << 2

namespace Ui {
	class SettingsDialog;
}

class SettingsDialog : public QDialog {
	Q_OBJECT

public:
	SettingsDialog(QWidget *parent = 0);
	~SettingsDialog();
	void setActiveTabs(unsigned int);

protected:
	void changeEvent(QEvent *e);

private:
	Ui::SettingsDialog *ui;
	void updateDevlist();
	bool ignore_change_event; //don't trigger event when adds items to combox

private slots:
	void on_pbClearCache_clicked();
 void on_pbFSConsole_clicked();
	void on_pbOpenLogDir_clicked();
	void on_cbDebug_clicked(bool checked);
	void on_cbRing_currentIndexChanged(int index);
	void on_cbOutput_currentIndexChanged(int index);
	void on_cbInput_currentIndexChanged(int index);
	void on_pbSave_clicked();
	void on_tbSelectFile_clicked();
	void on_pbLoopTest_clicked();
	void on_tbRingTest_clicked();
	void on_pbReset_clicked();
	void on_pbSaveGeneral_clicked();
	void on_pushButton_clicked();
};

#endif // SETTINGS_DIALOG_H
