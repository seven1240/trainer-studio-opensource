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

    tcp_client = new TCPClient();
    tcp_client->start();

    // We should not need set NULL manually, but without this,
    // flash_dialog is NULL but not login_dialog, weird
    login_dialog = NULL;
    flash_dialog = new FlashDialog(this);
    incoming_call_dialog = new IncomingCallDialog();
    settings_dialog = NULL;

    this->connect(tcp_client, SIGNAL(authenticated(QVariantMap)), this, SLOT(onAuthenticated(QVariantMap)));
    this->connect(tcp_client, SIGNAL(paused(bool)), this, SLOT(onPaused(bool)));
    this->connect(tcp_client, SIGNAL(forcedPause(QString)), this, SLOT(onForcedPause(QString)));
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
            login_dialog = new LoginDialog();
            connect(login_dialog, SIGNAL(Login()), this, SLOT(onLogin()));
    //        connect(preferences, SIGNAL(preprocessorsApplied(QStringList)), this, SLOT(applyPreprocessors(QStringList)));
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

void MainWindow::on_pushButton_2_clicked()
{
    fshost = new FSHost();

    qDebug() << "will start" << endl;
    fshost->start();
    qDebug() << "end start" << endl;

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
    show();
}

void MainWindow::on_btnState_clicked()
{
    if(ui->btnState->isChecked()){
        tcp_client->pause(false);
    }else{
        tcp_client->pause(true);
    }
//    ui->btnState->toggle();
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
    ui->lbStatus->setText(QString("SIP state: %1").arg(state));
}

void MainWindow::on_pushButton_3_clicked()
{
    ISettings *settings = new ISettings(this);
    settings->resetGateway();
    settings->saveToFile();
    delete settings;
}

void MainWindow::on_pushButton_4_clicked()
{
    if(!settings_dialog) settings_dialog = new SettingsDialog();
    settings_dialog->show();
}
