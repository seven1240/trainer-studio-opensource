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

class User;

class LoginDialog : public QDialog {
	Q_OBJECT

public:
	LoginDialog(QWidget *parent = 0);
	~LoginDialog();

protected:
	void changeEvent(QEvent *e);
	void closeEvent(QCloseEvent *e);
	void onKeyPressEvent(QKeyEvent *e);

private:
	QLineEdit *_leUsername;
	QLineEdit *_lePassword;
	QPushButton *_pbSettings;
	QPushButton *_pbLogin;
	QPushButton *_pbCancel;
	QLabel *_lbProgress;
	QTextEdit *_teProgress;
	QFrame *_loginFrame;
	QFrame *_progressFrame;
	bool _authenticated;
	bool _abort;
	User *_user;

	void abortLogin();
	void abortLogin(QString msg);

private:
	void showProgress();
	void showLogin();
	void setProgress(QString string);

public slots:

private slots:
	void onServerConnectionConnected();
	void onServerConnectionAuthenticated(User *user);
	void onServerConnectionDisconnected();
	void onSettingsClicked();
	void onAuthenticateError(QString);
	void onSocketError(QString);
	void onCancelClicked();
	void onLoginClicked();
	void onFSModuleLoaded(QString modType, QString modKey, QString modName);
};

#endif // LOGINDIALOG_H
