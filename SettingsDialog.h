#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SettingsDialog *ui;
    void updateDevlist();

    bool ignore_change_event; //don't trigger event when adds items to combox
private slots:
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

#endif // SETTINGSDIALOG_H
