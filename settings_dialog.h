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
