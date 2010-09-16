#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <flashdialog.h>
#include <logindialog.h>
#include <IncomingCallDialog.h>
#include <SettingsDialog.h>
#include "trainer_studio.h"
#include <QKeyEvent>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showLoginDialog();
    QVariantMap getUser() { return _user; }

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;

    FlashDialog *flash_dialog;
    LoginDialog *login_dialog;
    IncomingCallDialog *incoming_call_dialog;
    SettingsDialog *settings_dialog;
    QVariantMap _user;
    bool _sipStateReady;

    void keyPressEvent(QKeyEvent *event);


private slots:
    void on_pbConference_clicked();
    void on_pbEchoTest_clicked();
    void on_actionPreferences_triggered();
    void on_btnState_clicked();
    void on_pushButton_clicked();
    void onAuthenticated(QVariantMap);
    void onPaused(bool);
    void onForcedPause(QString reason);
    void onLogin();
    void onAnswered();
    void onGatewayStateChange(QString state);
    void onReservedForInteraction(QVariantMap);
    void onSocketDisconnected();
    void on_actionAbout_triggered();

};
#endif // MAINWINDOW_H
