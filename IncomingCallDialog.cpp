#include "IncomingCallDialog.h"
#include "ui_IncomingCallDialog.h"
#include "fshost.h"

IncomingCallDialog::IncomingCallDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IncomingCallDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->connect(fshost, SIGNAL(incomingCall(QSharedPointer<switch_event_t>)), this, SLOT(onIncomingCall(QSharedPointer<switch_event_t>)));

}

IncomingCallDialog::~IncomingCallDialog()
{
    delete ui;
}

void IncomingCallDialog::changeEvent(QEvent *e)
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

void IncomingCallDialog::onIncomingCall(QSharedPointer<switch_event_t>event)
{
    qDebug() << "incoming call";
    const char *cid_name = switch_event_get_header_nil(event.data(), "Caller-Caller-ID-Name");
    const char *cid_number = switch_event_get_header_nil(event.data(), "Caller-Caller-ID-Number");
    ui->lbCallerID->setText(QString("\"%1\" <%2>").arg(cid_name).arg(cid_number));
    show();
    raise();
}

void IncomingCallDialog::on_pbAnswer_clicked()
{
    QString res;
    fshost->sendCmd("pa", "answer", &res);
    emit(answered());
    hide();
}

void IncomingCallDialog::on_pbReject_clicked()
{
    QString res;
    fshost->sendCmd("pa", "hangup", &res);
    hide();
}
