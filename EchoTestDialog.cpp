#include "EchoTestDialog.h"
#include "ui_EchoTestDialog.h"
#include <qmessagebox.h>
#include "SettingsDialog.h"
#include "mainwindow.h"
#include "TCPClient.h"


EchoTestDialog::EchoTestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EchoTestDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Echo Test");
    ui->pbAdvanced->setVisible(false);

    QVariantMap user = ((MainWindow *)this->parent())->getUser();
    qDebug() << user;
    if (user["skip_echo_test"].toString() == "true") {
        ui->pbSkip->setVisible(true);
    } else {
        ui->pbSkip->setVisible(false);
    }

    this->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
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

void EchoTestDialog::closeEvent(QCloseEvent *e)
{
    //ESC cannot be caught by this???
    QString res;
    fshost->sendCmd("hupall", "", &res);
}

void EchoTestDialog::on_pbEchoTest_clicked()
{
    if (!fshost->isSofiaReady()) {
        ui->lbMessage->setText("Please wait a while and try again!");
        return;
    }

    if (ui->pbEchoTest->text() == "Finish") {
        QString res;
        fshost->sendCmd("hupall", "", &res);
        return;
    }

    ui->lbMessage->setText("Running");

    static int firstRun = 0;
    if (firstRun++ == 0) {
        this->connect(fshost, SIGNAL(newEvent(QSharedPointer<switch_event_t>)),
                      this, SLOT(onNewEvent(QSharedPointer<switch_event_t>)));
    }

    QString res;
    fshost->sendCmd("pa", "call echo", &res);
    qDebug() << res;
}

void EchoTestDialog::onNewEvent(QSharedPointer<switch_event_t> spEvent)
{
    switch_event_t *event = spEvent.data();

    qDebug() << event->event_id;

    switch(event->event_id) {
    case SWITCH_EVENT_CHANNEL_CREATE:
        {
            ui->lbMessage->setText("Dialing...");
            ui->pbEchoTest->setText("Finish");
            break;
        }
    case SWITCH_EVENT_CHANNEL_ANSWER:
        {
            ui->lbMessage->setText("Answered");
            break;
        }
    case SWITCH_EVENT_CHANNEL_BRIDGE:
        {
            ui->lbMessage->setText("Server Answered, Please talk...");
            break;
        }
    case SWITCH_EVENT_CHANNEL_HANGUP:
        {
            qDebug() << switch_event_get_header_nil(event, "Caller-Direction");
            if (!strcmp(switch_event_get_header_nil(event, "Caller-Direction"), "outbound")) {
                //wait for the the portaudio channel hangup
                return;
            }
            ui->lbMessage->setText("Channel Hangup!");
            ui->pbEchoTest->setText("Start Echo Test");
            int ret = QMessageBox::question(NULL, "Echo Test",
                                  "Test call finished, Did you hear yourself?",
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            if (ret == QMessageBox::Yes) {
                tcp_client->sendAction("Pause");
                close();
                return;
            } else {
                ui->lbMessage->setText("Please Try again, or trouble shoot by click the Advanced button");
                ui->pbAdvanced->setVisible(true);
            }
        }
    default: //Do nothing
        ;
    }
    ui->lbMessage->repaint();

}

void EchoTestDialog::on_pbSkip_clicked()
{
    tcp_client->sendAction("Pause");
    close();
}

void EchoTestDialog::on_pbAdvanced_clicked()
{
    SettingsDialog *settings_dialog = new SettingsDialog(this);
    // auto delete on close
    settings_dialog->setAttribute(Qt::WA_DeleteOnClose);
    settings_dialog->setActiveTabs(TAB_AUDIO);
    settings_dialog->show();
}
