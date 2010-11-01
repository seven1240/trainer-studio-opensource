#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <flashdialog.h>
#include <logindialog.h>
#include <IncomingCallDialog.h>
#include <SettingsDialog.h>
#include "trainer_studio.h"
#include <QKeyEvent>
#include "fshost.h"

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showLoginDialog();
    QVariantMap getUser() { return _user; }

protected:
    void changeEvent(QEvent *e);
    QLayout *createBody();

private:
    MainWindow *ui;
    QPushButton *btnState;
    QLabel *lbStatus;
    QLabel *lbSIPStatus;
    FlashDialog *flash_dialog;
    LoginDialog *login_dialog;
    IncomingCallDialog *incoming_call_dialog;
    SettingsDialog *settings_dialog;
    QVariantMap _user;
    bool _sipStateReady;
    QString _activeUUID;

    void keyPressEvent(QKeyEvent *event);
    void parseCallResult(QString res);

    QTimer *_timer;


private slots:
    void on_pbHupall_clicked();
    void on_pbConference_clicked();
    void on_pbEchoTest_clicked();
    void on_actionPreferences_triggered();
    void on_btnState_clicked();
    void on_pushButton_clicked();
    void on_actionAbout_triggered();
    void onAuthenticated(QVariantMap);
    void onPaused(bool);
    void onForcedPause(QString reason);
    void onLogin();
    void onAnswered(QString cid_name, QString cid_number);
    void onGatewayStateChange(QString state);
    void onReservedForInteraction(QVariantMap);
    void onSocketDisconnected();
    void onNewEvent(QSharedPointer<switch_event_t>);
    void onTimerTimeout();

};
#endif // MAINWINDOW_H
