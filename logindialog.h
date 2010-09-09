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
    QString _moduleLoadingMsg;
public slots:
    void onAuthenticated(QVariantMap);
    void onAuthenticateError(QString);

private slots:
    void on_cancelLogin_clicked();
    void on_btnLogin_clicked();
    void on_pushButton_clicked();
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
    void onFSModuleLoaded(QString, QString);
};

#endif // LOGINDIALOG_H
