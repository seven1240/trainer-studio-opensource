#include <QDebug.h>
#include <QMessageBox>
#include <QApplication>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QGroupBox>
#include <QtGui/QTextEdit>
#include <QtGui/QPushButton>
#include "application_controller.h"
#include "login_dialog.h"
#include "server_connection.h"
#include "settings_dialog.h"
#include "echo_test_dialog.h"
#include "progress_widget.h"
#include "utils.h"

LoginDialog::LoginDialog(ProgressWidget *progress, QWidget *parent) :
	QDialog(parent)
{
	_settings = new QPushButton("Settings");
	_settings->setObjectName("Settings");
	_login = new QPushButton("Login");
	_login->setObjectName("Login");
	_username = new QLineEdit();
	_password = new QLineEdit();
	_password->setEchoMode(QLineEdit::Password);
	_progress = progress;

	QGroupBox *loginFormGroupBox = new QGroupBox(tr("Login"));
	QFormLayout *formLayout = new QFormLayout;
	formLayout->addRow(new QLabel(tr("Username:")), _username);
	formLayout->addRow(new QLabel(tr("Password:")), _password);
	loginFormGroupBox->setLayout(formLayout);

	_loginFrame = new QFrame();
	QVBoxLayout *loginLayout = new QVBoxLayout();
	loginLayout->addWidget(loginFormGroupBox);
	loginLayout->addWidget(_login);
	loginLayout->addWidget(_settings);
	_loginFrame->setLayout(loginLayout);

	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->addWidget(_loginFrame);
	mainLayout->addWidget(_progress);
	setLayout(mainLayout);

	setFixedSize(320, 240);
	setWindowTitle("Login");
	Utils::centerWindowOnDesktop(this);

	showLogin();

	QSettings settings;
	_authenticated = false;

	QString username = settings.value("StoredData/Username", "").toString();
	_username->setText(username);
	if (username.length() > 0)
		_password->setFocus();

	connect(ApplicationController::server(), SIGNAL(connected()), this, SLOT(onServerConnectionConnected()));
	connect(ApplicationController::server(), SIGNAL(authenticated(User*)), this, SLOT(onServerConnectionAuthenticated(User*)));
	connect(ApplicationController::server(), SIGNAL(disconnected()), this, SLOT(onServerConnectionDisconnected()));
	connect(ApplicationController::server(), SIGNAL(authenticateError(QString)), this, SLOT(onAuthenticateError(QString)));
	connect(ApplicationController::server(), SIGNAL(socketError(QString)), this, SLOT(onSocketError(QString)));
	connect(_login, SIGNAL(clicked()), this, SLOT(onLoginClicked()));
	connect(_settings, SIGNAL(clicked()), this, SLOT(onSettingsClicked()));
	connect(_progress, SIGNAL(canceled()), this, SLOT(onCancelClicked()));

	QMetaObject::connectSlotsByName(this);
}

LoginDialog::~LoginDialog()
{
}

void LoginDialog::onServerConnectionConnected()
{
	ApplicationController::server()->login(_username->text(), _password->text());
}

void LoginDialog::onServerConnectionAuthenticated(User *user)
{
	_user = user;
	_authenticated = true;
	ApplicationController::fs()->setupGateway(user->getVoipUsername(), user->getVoipPassword(), user->getVoipServer(), false);
}

void LoginDialog::onServerConnectionDisconnected()
{
	showLogin();
}

void LoginDialog::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		// retranslateUi(this);
		break;
	default:
		break;
	}
}

void LoginDialog::closeEvent(QCloseEvent * /*e*/)
{
}

void LoginDialog::onKeyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Escape) return;
}

void LoginDialog::abortLogin()
{
	_abort = true;
	showLogin();
}

void LoginDialog::abortLogin(QString msg)
{
	abortLogin();
	if (_abort || _authenticated)
		return;
	QMessageBox::critical(this, QApplication::applicationName(), msg);
}

void LoginDialog::onCancelClicked()
{
	qDebug() << "Cancel";
	ApplicationController::server()->close();
	abortLogin();
}

void LoginDialog::onAuthenticateError(QString reason)
{
	ApplicationController::server()->close();
	QString msg = QString("Authenticate Error:\nReason: %1").arg(reason);
	abortLogin(msg);
}

void LoginDialog::onSocketError(QString error)
{
	abortLogin(error);
}

void LoginDialog::showProgress()
{
	_loginFrame->hide();
	_progress->show();
}

void LoginDialog::showLogin()
{
	_progress->hide();
	_loginFrame->show();
}

void LoginDialog::onLoginClicked()
{
	QSettings settings;
	settings.setValue("StoredData/Username", _username->text());
	QString host = settings.value("General/trainer_server").toString();
	int port = settings.value("trainer_server_port").toInt();
	if (host == NULL || host.isEmpty()) host = "voip.idapted.com";
	if (port == 0) port = 7000;

	_abort = false;
	showProgress();
	ApplicationController::server()->open(host, port);
}

void LoginDialog::onSettingsClicked()
{
	SettingsDialog *settings_dialog = new SettingsDialog(this);
	settings_dialog->setAttribute(Qt::WA_DeleteOnClose);
	settings_dialog->show();
}