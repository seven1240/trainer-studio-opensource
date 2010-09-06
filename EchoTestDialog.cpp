#include "EchoTestDialog.h"
#include "ui_EchoTestDialog.h"

EchoTestDialog::EchoTestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EchoTestDialog)
{
    ui->setupUi(this);
}

EchoTestDialog::~EchoTestDialog()
{
    delete ui;
}

void EchoTestDialog::changeEvent(QEvent *e)
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
