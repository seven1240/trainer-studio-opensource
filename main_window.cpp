#include <QtGui>
#include <QDebug.h>
#include <QMessageBox>
#include "application_controller.h"
#include "server_connection.h"
#include "main_window.h"
#include "isettings.h"
#include "utils.h"

MainWindow::MainWindow(QWidget *parent) :
	QWidget(parent)
{
	createBody();

	setWindowTitle("Trainer Studio - Idapted Ltd.");
	setFixedSize(228, 350);
	Utils::centerWindowOnDesktop(this);

	_sipStateReady = false;
	_state->setChecked(false);
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
	QPushButton *stateButton = new QPushButton("|| Pause");
	QPushButton *hangupButton = new QPushButton("Hangup");
	QPushButton *closeButton = new QPushButton("Close");
	QPushButton *aboutButton = new QPushButton("About");
	QLabel *sipLabel = new QLabel("SIP: None");

	settingsButton->setObjectName("Settings");
	logoutButton->setObjectName("Logout");
	testEchoButton->setObjectName("Echo");
	testFlashButton->setObjectName("Flash");
	stateButton->setObjectName("State");
	hangupButton->setObjectName("Hangup");
	closeButton->setObjectName("Close");
	aboutButton->setObjectName("About");

	stateButton->setCheckable(true);
	settingsButton->setVisible(false);
	testFlashButton->setVisible(ApplicationController::isDebugging());

	QGroupBox *commonBox = new QGroupBox("Trainer");
	QVBoxLayout *commonLayout = new QVBoxLayout();
	commonLayout->addWidget(hangupButton);
	commonLayout->addWidget(stateButton);
	commonLayout->addWidget(logoutButton);
	commonLayout->addWidget(settingsButton);
	commonLayout->addWidget(closeButton);
	commonBox->setLayout(commonLayout);

	QGroupBox *testsBox = new QGroupBox("Diagnostics");
	QVBoxLayout *testsLayout = new QVBoxLayout();
	testsLayout->addWidget(testEchoButton);
	testsLayout->addWidget(testFlashButton);
	testsLayout->addWidget(aboutButton);
	testsBox->setLayout(testsLayout);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(commonBox);
	layout->addWidget(testsBox);
	layout->addWidget(sipLabel);
	setLayout(layout);

	_state = stateButton;
	_sipStatusLabel = sipLabel;

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

void MainWindow::on_Close_clicked()
{
	close();
}

void MainWindow::on_Logout_clicked()
{
	ApplicationController::server()->logout();
}

void MainWindow::on_State_clicked()
{
	ApplicationController::server()->pause(_state->isChecked());
}

void MainWindow::onPaused(bool state)
{
	_state->setChecked(state);
	if (state) {
		_state->setText("> Start Working");
		QApplication::alert(this, 0);
		_timer->start();
	}
	else {
		_state->setText("|| Pause");
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
		}
	}
	else if (state == "TRYING" || state == "REGISTER") {
		// do nothing
	}
	else { //UNREGED UNREGISTER FAILED FAIL_WAIT EXPIRED NOREG NOAVAIL
		if (_sipStateReady) {
			_sipStateReady = false;
			if (_state->isChecked())
				ApplicationController::server()->pause(true); //force pause
		}
	}
	_sipStatusLabel->setText(QString("SIP State: %1").arg(state));
}

void MainWindow::onReservedForInteraction(QVariantMap data)
{
	QString msg = QString("New learner incoming with interaction #%1").arg(data["interaction_id"].toString());
	_systemTray->showMessage(QApplication::applicationName(), msg, QSystemTrayIcon::Information, 3000);
}

void MainWindow::on_About_clicked()
{
	QMessageBox::about(this, QApplication::applicationName(),
					   QString("Version: %1\n\nCopyright (C): Idapted Ltd.").arg(QApplication::applicationVersion()));
}

void MainWindow::on_Echo_clicked()
{
	emit testEcho();
}

void MainWindow::on_Conference_clicked()
{
	ApplicationController::fs()->call("conf");
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	if (false) return; // TODO if in call?

	if (event->key() == 35 || event->key() == Qt::Key_Asterisk ||  // # *
		event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9 ||  // 0 - 9
		event->key() >= Qt::Key_A && event->key() <= Qt::Key_D ) { // A-D
		ApplicationController::fs()->portAudioDtmf((char)event->key());
	}
}

void MainWindow::on_Hangup_clicked()
{
	ApplicationController::fs()->hangup(true);
}

void MainWindow::onTimer()
{
	_systemTray->showMessage(QApplication::applicationName(), "Trainer Studio Paused!", QSystemTrayIcon::Information, 1000);
}
