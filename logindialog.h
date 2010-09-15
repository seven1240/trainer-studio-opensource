#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <qvariant.h>

namespace Ui {
    class LoginDialog;
}

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

protected:
    void changeEvent(QEvent *e);

signals:
    void login();

private:
    Ui::LoginDialog *ui;
    QVariantMap _user;
    bool _authenticated;
    bool _abort; //abort login

    void abortLogin();
    void abortLogin(QString msg);

public slots:

private slots:
    void on_pbSettings_clicked();
    void onAuthenticated(QVariantMap);
    void onAuthenticateError(QString);
    void onAuthenticateTimeout();
    void onSocketError(QString);
    void on_cancelLogin_clicked();
    void on_btnLogin_clicked();
    void onFSModuleLoaded(QString modType, QString modKey, QString modName);
    void doRegisterToVoIP();

};

#endif // LOGINDIALOG_H
