#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include "fshost.h"
#include "TcpClient.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // We should not need set NULL manually, but without this,
    // flash_dialog is NULL but not login_dialog, weird
    login_dialog = NULL;
    flash_dialog = NULL;

    tcp_client = new TCPClient();
    tcp_client->start();

}


MainWindow::~MainWindow()
{
    delete ui;
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
    qDebug() << "hahahahah" << endl;

    if (!flash_dialog) {
        flash_dialog = new FlashDialog();
//        connect(preferences, SIGNAL(preprocessorsApplied(QStringList)), this, SLOT(applyPreprocessors(QStringList)));
    }

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
