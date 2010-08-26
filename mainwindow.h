#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <flashdialog.h>
#include <logindialog.h>
#include <IncomingCallDialog.h>
#include "trainer_studio.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    void showLoginDialog();
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;

    FlashDialog *flash_dialog;
    LoginDialog *login_dialog;
    IncomingCallDialog *incoming_call_dialog;

private slots:
    void on_btnState_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void onAuthenticated(QVariantMap);
    void onPaused(bool);
    void onForcedPause(QString reason);
    void onLogin();
    void onAnswered();
};

#endif // MAINWINDOW_H
