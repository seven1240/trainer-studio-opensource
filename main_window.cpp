#include <QtGui>
#include <QDebug.h>
#include <QMessageBox>
#include "application_controller.h"
#include "server_connection.h"
#include "main_window.h"
#include "isettings.h"
#include "utils.h"
#include "settings_dialog.h"

MainWindow::MainWindow(QWidget *parent) :
	QWidget(parent)
{
	createBody();

	setWindowTitle("Trainer Studio - Eleutian Inc.");
	setFixedSize(228, 350);
	Utils::centerWindowOnDesktop(this);

	_isPaused = true;
	_sipStateReady = false;
	_timer = new QTimer(this);
	_timer->setInterval(20000);

	_systemTray = new QSystemTrayIcon(QIcon(":/images/taskbar_icon"), this);
	_systemTray->setToolTip(QApplication::applicationName());
	_systemTray->show();
	_systemTray->showMessage(QApplication::applicationName(), "Initialized", QSystemTrayIcon::Information, 2000);

	connect(ApplicationController::server(), SIGNAL(pauseChanged(bool)), this, SLOT(onPaused(bool)));
	connect(ApplicationController::server(), SIGNAL(forcedPause(QString)), this, SLOT(onForcedPause(QString)));
	connect(ApplicationController::server(), SIGNAL(reservedForInteraction(QVariantMap)), this, SLOT(onReservedForInteraction(QVariantMap)));
	connect(ApplicationController::fs(), SIGNAL(gatewayStateChange(QString, QString)), this, SLOT(onGatewayStateChange(QString, QString)));
	connect(_timer, SIGNAL(timeout()), this, SLOT(onTimer()));

	QMetaObject::connectSlotsByName(this);
}

QLayout *MainWindow::createBody()
{
	QPushButton *settingsButton = new QPushButton("Settings");
	QPushButton *logoutButton = new QPushButton("Logout");
	QPushButton *testEchoButton = new QPushButton("Echo");
	QPushButton *testFlashButton = new QPushButton("Flash");
	QPushButton *callButton = new QPushButton("Call");
	QPushButton *stateButton = new QPushButton("--initializing--");
	QPushButton *hangupButton = new QPushButton("Hangup");
	QPushButton *aboutButton = new QPushButton("About");
	QLabel *sipLabel = new QLabel("SIP: None");
	QToolButton *sipReg = new QToolButton();
	sipReg->setText("Register");

	settingsButton->setObjectName("Settings");
	callButton->setObjectName("Call");
	logoutButton->setObjectName("Logout");
	testEchoButton->setObjectName("Echo");
	testFlashButton->setObjectName("Flash");
	stateButton->setObjectName("State");
	hangupButton->setObjectName("Hangup");
	aboutButton->setObjectName("About");
	sipReg->setObjectName("SipReg");

	settingsButton->setVisible(true);
	testFlashButton->setVisible(ApplicationController::isDebugging());

	QGroupBox *trainingBox = new QGroupBox("Training");
	QVBoxLayout *trainingLayout = new QVBoxLayout();
	trainingLayout->addWidget(stateButton);
	trainingBox->setLayout(trainingLayout);

	QGroupBox *commonBox = new QGroupBox("Telephony");
	QVBoxLayout *commonLayout = new QVBoxLayout();
	commonLayout->addWidget(callButton);
	commonLayout->addWidget(hangupButton);
	commonLayout->addWidget(logoutButton);
	commonBox->setLayout(commonLayout);

	QGroupBox *testsBox = new QGroupBox("Diagnostics");
	QVBoxLayout *testsLayout = new QVBoxLayout();
	testsLayout->addWidget(settingsButton);
	// testsLayout->addWidget(testEchoButton);
	testsLayout->addWidget(testFlashButton);
	testsLayout->addWidget(aboutButton);
	testsBox->setLayout(testsLayout);

	QStatusBar *statusBar = new QStatusBar();
	statusBar->setSizeGripEnabled(false);
	
	statusBar->addWidget(sipLabel);
	statusBar->addWidget(sipReg);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(trainingBox);
	layout->addWidget(commonBox);
	layout->addWidget(testsBox);
	layout->addWidget(statusBar);
	setLayout(layout);

	_state = stateButton;
	_sipStatusLabel = sipLabel;
	_sipReg = sipReg;

	return layout;
}

MainWindow::~MainWindow()
{
	delete _timer;
}

void MainWindow::showEvent(QShowEvent * /*e*/)
{
}

void MainWindow::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		// retranslateUi(this);
		break;
	default:
		break;
	}
}

void MainWindow::on_Flash_clicked()
{
	emit testFlash();
}

void MainWindow::on_Logout_clicked()
{
	ApplicationController::server()->logout();
}

void MainWindow::on_State_clicked()
{
	if (!_sipStateReady) switch_sleep(2000000);

	if (!_sipStateReady){
		QMessageBox::warning(this, QApplication::applicationName(),
			"SIP not Ready! \nPlease wait SIP:REGED or click the [Register] button to register immediately");
		return;
	}
	ApplicationController::server()->pause(!_isPaused);
}

void MainWindow::onPaused(bool state)
{
	_isPaused = state;
	if (state) {
		_state->setText("> Start Working");
		_state->setStyleSheet("*{background-color:red; color:white}");
		QApplication::alert(this, 0);
		_timer->start();
	}
	else {
		_state->setText("|| Pause");
		_state->setStyleSheet("*{background-color:green; color:white}");
		_timer->stop();
	}
}

void MainWindow::onForcedPause(QString reason)
{
	QMessageBox::warning(this, QApplication::applicationName(), reason);
	onPaused(true);
}

void MainWindow::onGatewayStateChange(QString /*name*/, QString state)
{
	if (state == "REGED") {
		if (!_sipStateReady) {
			_sipStateReady = true;
			_sipReg->setVisible(false);
		}
	}
	else if (state == "TRYING" || state == "REGISTER") {
		// do nothing
	}
	else { //UNREGED UNREGISTER FAILED FAIL_WAIT EXPIRED NOREG NOAVAIL
		if (_sipStateReady) {
			_sipStateReady = false;
			if (!_isPaused)
				ApplicationController::server()->pause(true); //force pause
		}
		_sipReg->setVisible(true);
	}
	_sipStatusLabel->setText(QString("SIP: %1").arg(state));
}

void MainWindow::onReservedForInteraction(QVariantMap data)
{
	QString msg = QString("New learner incoming with interaction #%1").arg(data["interaction_id"].toString());
	_systemTray->showMessage(QApplication::applicationName(), msg, QSystemTrayIcon::Information, 3000);
}

void MainWindow::on_About_clicked()
{
	QMessageBox::about(this, QApplication::applicationName(),
		QString("%1 Version: %2\n\nCopyright (C) 2010 Eleutian Inc. \n"
			"All rights reserved.").arg(
			QApplication::applicationName()).arg(
			QApplication::applicationVersion()));
}

void MainWindow::on_Echo_clicked()
{
	emit testEcho();
}

void MainWindow::on_Call_clicked()
{
	emit call();
}

void MainWindow::on_Hangup_clicked()
{
	ApplicationController::fs()->hangup(true);
}

void MainWindow::on_Settings_clicked()
{
	SettingsDialog *settings_dialog = new SettingsDialog(this);
	settings_dialog->setAttribute(Qt::WA_DeleteOnClose);
	settings_dialog->show();
}

void MainWindow::on_SipReg_clicked()
{
	ApplicationController::fs()->registerSIP();
}

void MainWindow::onTimer()
{
	_systemTray->showMessage(QApplication::applicationName(), "Trainer Studio Paused!", QSystemTrayIcon::Information, 1000);
}
