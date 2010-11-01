#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QKeyEvent>
#include <QVariant>

QT_BEGIN_NAMESPACE
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QFrame;
QT_END_NAMESPACE

class LoginDialog : public QDialog {
  Q_OBJECT

public:
  LoginDialog(QWidget *parent = 0);
  ~LoginDialog();

protected:
  void changeEvent(QEvent *e);
  void closeEvent(QCloseEvent *);
  void KeyPressEvent(QKeyEvent *e);

private:
  QLineEdit *_leUsername;
  QLineEdit *_lePassword;
  QPushButton *_pbSettings;
  QPushButton *_pbLogin;
  QPushButton *_pbCancel;
  QLabel *_lbProgress;
  QTextEdit *_teProgress;
  QVariantMap _user;
  QFrame *_loginFrame;
  QFrame *_progressFrame;
  bool _authenticated;
  bool _abort;

  void abortLogin();
  void abortLogin(QString msg);

private:
  void showProgress();
  void showLogin();
  void setProgress(QString string);

public slots:

private slots:
  void onSettingsClicked();
  void onAuthenticated(QVariantMap);
  void onAuthenticateError(QString);
  void onAuthenticateTimeout();
  void onSocketError(QString);
  void onCancelClicked();
  void onLoginClicked();
  void onFSModuleLoaded(QString modType, QString modKey, QString modName);
  void doRegisterToVoIP();
};

#endif // LOGINDIALOG_H
