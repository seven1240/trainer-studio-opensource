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
    void Login();

private:
    Ui::LoginDialog *ui;
    QVariantMap _user;
    bool _authenticated;
    bool _abort; //abort login

public slots:

private slots:
    void onAuthenticated(QVariantMap);
    void onAuthenticateError(QString);
    void onAuthenticateTimeout();
    void on_cancelLogin_clicked();
    void on_btnLogin_clicked();
    void onFSModuleLoaded(QString, QString);
    void doRegisterToVoIP();

};

#endif // LOGINDIALOG_H
