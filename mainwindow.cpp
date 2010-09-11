#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include "fshost.h"
#include "TcpClient.h"
#include "isettings.h"
#include "qmessagebox.h"

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

    this->connect(tcp_client, SIGNAL(authenticated(QVariantMap)), this, SLOT(onAuthenticated(QVariantMap)));
    this->connect(tcp_client, SIGNAL(paused(bool)), this, SLOT(onPaused(bool)));
    this->connect(tcp_client, SIGNAL(forcedPause(QString)), this, SLOT(onForcedPause(QString)));
    this->connect(tcp_client, SIGNAL(reservedForInteraction(QVariantMap)), this, SLOT(onReservedForInteraction(QVariantMap)));
    this->connect(tcp_client, SIGNAL(socketDisconnected()), this, SLOT(onSocketDisconnected()));
    this->connect(incoming_call_dialog, SIGNAL(answered()), this, SLOT(onAnswered()));
    this->connect(fshost, SIGNAL(gatewayStateChange(QString)), this, SLOT(onGatewayStateChange(QString)));

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
    }else{
        ui->btnState->setText("|| Pause");
        ui->btnState->setChecked(true);
    }
}

void MainWindow::onForcedPause(QString reason)
{
    QMessageBox::warning(this, QApplication::applicationName(),
                          reason);
    ui->btnState->setText("> Start Working");
    ui->btnState->setChecked(false);
}

void MainWindow::onAnswered()
{
    flash_dialog -> raise();
    flash_dialog -> show();
    flash_dialog -> activateWindow();
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
    ui->lbStatus->setText(QString("New learner comming with InteractionID %1").arg(data["interaction_id"].toString()));
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
    QString res;
    ui->lbStatus->setText("Echo test");
    fshost->sendCmd("pa", "call echo", &res);
}
