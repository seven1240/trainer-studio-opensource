#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

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
class ProgressWidget;

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
	ProgressWidget *_progress;
	QLineEdit *_username;
	QLineEdit *_password;
	QPushButton *_settings;
	QPushButton *_login;
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
};

#endif // LOGIN_DIALOG_H
