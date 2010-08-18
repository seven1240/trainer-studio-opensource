#include "logindialog.h"
#include "ui_logindialog.h"
#include "qdebug.h"
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    qDebug() << "LoginDialog starting";
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void LoginDialog::on_buttonBox_accepted()
{
    qDebug() << "Accepted";
    emit Login();
}

void LoginDialog::on_buttonBox_rejected()
{
    qDebug() << "Rejected";
    this->show();
}

void LoginDialog::on_pushButton_clicked()
{
    emit Login();
}
