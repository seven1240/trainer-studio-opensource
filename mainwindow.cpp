#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include "TcpClient.h"
#include "isettings.h"
#include "qmessagebox.h"

QSystemTrayIcon *sysTray;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Trainer Studio - Idapted Ltd.");
//    ui->lbSIPStatus->setMinimumSize(ui->lbSIPStatus->sizeHint());
    statusBar()->addWidget(ui->lbSIPStatus);
    ui->lbStatus->setIndent(5);
    statusBar()->addWidget(ui->lbStatus, 1);

    tcp_client = new TCPClient();
    tcp_client->start();

    // We should not need set NULL manually, but without this,
    // flash_dialog is NULL but not login_dialog, weird
    // Also, Mac & Win have different default value
    login_dialog = NULL;
    flash_dialog = new FlashDialog(this);
    incoming_call_dialog = new IncomingCallDialog();
    settings_dialog = NULL;
    _sipStateReady = false;
    ui->btnState->setChecked(false);
    _timer = new QTimer(this);
    _timer->setInterval(20000);

    sysTray = new QSystemTrayIcon(QIcon(":/images/taskbar_icon"), this);
    sysTray->setToolTip(QApplication::applicationName());
    sysTray->show();
    sysTray->showMessage(QApplication::applicationName(), "Initialized", QSystemTrayIcon::Information, 2000);

    this->connect(tcp_client, SIGNAL(authenticated(QVariantMap)), this, SLOT(onAuthenticated(QVariantMap)));
    this->connect(tcp_client, SIGNAL(paused(bool)), this, SLOT(onPaused(bool)));
    this->connect(tcp_client, SIGNAL(forcedPause(QString)), this, SLOT(onForcedPause(QString)));
    this->connect(tcp_client, SIGNAL(reservedForInteraction(QVariantMap)), this, SLOT(onReservedForInteraction(QVariantMap)));
    this->connect(tcp_client, SIGNAL(socketDisconnected()), this, SLOT(onSocketDisconnected()));
    this->connect(incoming_call_dialog, SIGNAL(answered(QString, QString)), this, SLOT(onAnswered(QString, QString)));
    this->connect(fshost, SIGNAL(gatewayStateChange(QString)), this, SLOT(onGatewayStateChange(QString)));
    this->connect(_timer, SIGNAL(timeout()), this, SLOT(onTimerTimeout()));

}


MainWindow::~MainWindow()
{
    delete ui;
    if(login_dialog) delete login_dialog;
    if(flash_dialog) delete flash_dialog;
    if(incoming_call_dialog) delete incoming_call_dialog;
    if(settings_dialog) delete settings_dialog;
    if(tcp_client) delete tcp_client;
    if(fshost) delete fshost;
    delete(_timer);
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::showLoginDialog()
{
    if (!login_dialog) {
        login_dialog = new LoginDialog(this);
        connect(login_dialog, SIGNAL(login()), this, SLOT(onLogin()));
    }
    login_dialog -> raise();
//    login_dialog->setModal(true);
    login_dialog -> show();
    login_dialog -> activateWindow();

}

void MainWindow::on_pushButton_clicked()
{
    flash_dialog -> raise();
    flash_dialog -> show();
    flash_dialog -> activateWindow();
}

void MainWindow::onLogin()
{
    qDebug() << "LoginSuccess";
    show();
}

void MainWindow::onAuthenticated(QVariantMap user)
{
    //remember user info
    _user = user;
//    show();
}

void MainWindow::on_btnState_clicked()
{
    if (!_sipStateReady) {
        QMessageBox::warning(this, QApplication::applicationName(),
                             "Cannot Pause, VoIP not ready!");
    }
    // why isChecked shows reversed?
    qDebug() << ui->btnState->isChecked();
    tcp_client->pause(!ui->btnState->isChecked());
}

void MainWindow::onPaused(bool state)
{
    if(state){
        ui->btnState->setText("> Start Working");
        ui->btnState->setChecked(false);
        QApplication::alert(this, 0);
        _timer->start();
    }else{
        ui->btnState->setText("|| Pause");
        ui->btnState->setChecked(true);
        _timer->stop();
    }
}

void MainWindow::onForcedPause(QString reason)
{
    QMessageBox::warning(this, QApplication::applicationName(),
                          reason);
    onPaused(true);
}

void MainWindow::onAnswered(QString cid_name, QString cid_number)
{
    if (cid_name.left(2) == "IT") {
        //Interaction
        flash_dialog -> raise();
        flash_dialog -> show();
        flash_dialog -> activateWindow();
    } else {
        //Other
    }
}

void MainWindow::onGatewayStateChange(QString state)
{
    if (state == "REGED") {
        if (!_sipStateReady) {
            _sipStateReady = true;
        }
    } else if (state == "TRYING" || state == "REGISTER") {
        // do nothing
    } else { //UNREGED UNREGISTER FAILED FAIL_WAIT EXPIRED NOREG NOAVAIL
        if (_sipStateReady) {
            _sipStateReady = false;
            if (ui->btnState->isChecked()) tcp_client->pause(true); //force pause
        }
    }
    ui->lbSIPStatus->setText(QString("SIP State: %1").arg(state));
}

void MainWindow::onReservedForInteraction(QVariantMap data)
{
    QString msg = QString("New learner comming with InteractionID %1").arg(data["interaction_id"].toString());
    ui->lbStatus->setText(msg);
    sysTray->showMessage(QApplication::applicationName(),msg, QSystemTrayIcon::Information, 3000);
}

void MainWindow::onSocketDisconnected()
{
    QMessageBox::critical(this,QApplication::applicationName(), "Socket Broken!!");
    showLoginDialog();
    this->hide();
    delete flash_dialog;
    flash_dialog = new FlashDialog(this);
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, QApplication::applicationName(),
                             QString("Version: %1\n\nCopyright(c): Idapted Ltd.").arg(QApplication::applicationVersion()));
}

void MainWindow::on_actionPreferences_triggered()
{
    if(!settings_dialog) settings_dialog = new SettingsDialog();
    settings_dialog->show();
}

void MainWindow::on_pbEchoTest_clicked()
{
    if (!_activeUUID.isEmpty()) return;
    QString res;
    ui->lbStatus->setText("Echo test");
    fshost->sendCmd("pa", "call echo", &res);
    parseCallResult(res);
}

void MainWindow::on_pbConference_clicked()
{
    if (!_activeUUID.isEmpty()) return;
    QString res;
    ui->lbStatus->setText("Conference");
    fshost->sendCmd("pa", "call conf", &res);
    parseCallResult(res);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (false) return; // if in call?

    if (event->key() == 35 || event->key() == Qt::Key_Asterisk || //# *
        event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9 || //0 - 9
        event->key() >= Qt::Key_A && event->key() <= Qt::Key_D ) {// A-D

        QString params = QString("dtmf %1").arg((char)(event->key()));
        QString res;
        fshost->sendCmd("pa", params.toAscii(), &res);
    }
    qDebug() << "Key pressed: " << event->key();
}

void MainWindow::onNewEvent(QSharedPointer<switch_event_t> spEvent)
{
    switch_event_t *event = spEvent.data();

    QString uuid = switch_event_get_header_nil(event, "Unique-ID");

    if (uuid != _activeUUID )  return;

    QString eventName = switch_event_get_header_nil(event, "Event-Name");
    QString eventSubclass = switch_event_get_header_nil(event, "Event-Subclass");
    ui->lbStatus->setText(eventName + "::" + eventSubclass);

    switch(event->event_id) {
    case SWITCH_EVENT_CHANNEL_HANGUP_COMPLETE:
        {
            _activeUUID = "";
        }
    }
}

void MainWindow::parseCallResult(QString res)
{
    qDebug() << "Parse: " << res;
    QStringList sl = res.split(":");
    if (sl.count() == 3 && sl.at(0) == "SUCCESS") {
        _activeUUID = sl.at(2).trimmed();
        this->connect(fshost, SIGNAL(newEvent(QSharedPointer<switch_event_t>)),
                      this, SLOT(onNewEvent(QSharedPointer<switch_event_t>)));
    } else {
        _activeUUID = "";
        this->disconnect(this, SLOT(onNewEvent(QSharedPointer<switch_event_t>)));
        ui->lbStatus->setText(res.trimmed());
    }
}

void MainWindow::on_pbHupall_clicked()
{
    QString res;
    fshost->sendCmd("hupall", "", &res);
}

void MainWindow::onTimerTimeout()
{
    sysTray->showMessage(QApplication::applicationName(), "Trainer Studio Paused!", QSystemTrayIcon::Information, 1000);
}
