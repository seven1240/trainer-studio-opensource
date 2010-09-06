#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"
#include "trainer_studio.h"
#include "fshost.h"
#include <QtXml>
#include <qfiledialog.h>
#include "isettings.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    ui->cbTemplate->addItem("EQEnglish Trainer");
    ui->cbTemplate->addItem("Test Server");
    ui->cbTemplate->addItem("Developer");
    ui->cbTemplate->setCurrentIndex(-1);

    QSettings settings;
    settings.beginGroup("General");
    ui->leURL->setText(settings.value("url").toString());
    ui->leServer->setText(settings.value("trainer_server").toString() +
                          ":" + settings.value("trainer_server_port").toString());
    settings.endGroup();

    updateDevlist();

}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::changeEvent(QEvent *e)
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

void SettingsDialog::on_pushButton_clicked()
{
    switch(ui->cbTemplate->currentIndex()) {

    case 0:
        ui->leURL->setText("http://www.eqenglish.com");
        ui->leServer->setText("voip.idapted.com:7000");
        break;
    case 1:
        ui->leURL->setText("http://www.idapted.com");
        ui->leServer->setText("voip.idapted.com:7000");
        break;
    case 2:
        ui->leURL->setText("http://www.veecue.com");
        ui->leServer->setText("voip.veecue.com:7000");
        break;
    default:
        break;
    }
}

void SettingsDialog::on_pbSaveGeneral_clicked()
{
    QSettings settings;
    settings.beginGroup("General");
    settings.setValue("url", ui->leURL->text());
    QStringList slServer = ui->leServer->text().split(":");
    settings.setValue("trainer_server", slServer.at(0));
    settings.setValue("trainer_server_port", slServer.at(1) == "" ? "7000" : slServer.at(1) );
    settings.endGroup();
}

void SettingsDialog::updateDevlist()
{
    QString devices;

    fshost->sendCmd("pa", "rescan", &devices);
    switch_status_t status = fshost->sendCmd("pa", "devlist", &devices);

    if(SWITCH_STATUS_SUCCESS != status) {
        qDebug() << "PA devlist error: " << devices;
        return;
    }

    qDebug() << devices;

    ui->cbInput->clear();
    ui->cbOutput->clear();
    ui->cbRing->clear();

    QStringList devlist = devices.split("\n");
    for(int i=0; i<devlist.count(); i++) {
        QStringList fieldlist = devlist.at(i).split(";");

        if(fieldlist.count()< 4) continue;

        QString id = fieldlist.at(0);
        QString name = fieldlist.at(1);
        QString inputs = fieldlist.at(2);
        QString outputs = fieldlist.at(3);

        if(inputs.toInt() > 0 ) {
            ui->cbInput->addItem(name, id);
        }else if(outputs.toInt() > 0) {
            ui->cbOutput->addItem(name, id);
            ui->cbRing->addItem(name, id);
        }

        if(fieldlist.count() > 4) {
            QString flag = fieldlist.at(4);
            QStringList flaglist = flag.split(",");
            if(flaglist.count() > 1) {
                if("r" == flaglist.at(0) || "r" == flaglist.at(1)) {
                    ui->cbRing->setCurrentIndex(ui->cbRing->count() - 1);
                }else if ("o" == flaglist.at(0) || "o" == flaglist.at(1)) {
                    ui->cbOutput->setCurrentIndex(ui->cbOutput->count() - 1);
                }else if ("i" == flaglist.at(0) || "i" == flaglist.at(1)) {
                    ui->cbInput->setCurrentIndex(ui->cbInput->count() - 1);
                }
            }
        }
    }

    ISettings *isettings = new ISettings(this);
    ui->leRingFile->setText(isettings->getPaRingFile());
    delete isettings;
}

void SettingsDialog::on_pbReset_clicked()
{
    updateDevlist();
}

void SettingsDialog::on_tbRingTest_clicked()
{
    QString res;
    fshost->sendCmd("pa", QString("play %1").arg(ui->leRingFile->text()).toAscii(), &res);
}

void SettingsDialog::on_pbLoopTest_clicked()
{
    QString res;
    fshost->sendCmd("pa", "looptest", &res);
}

void SettingsDialog::on_tbSelectFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                       tr("Open File"), QDir::home().absolutePath(), tr("Wav Files (*.wav)"));
    ui->leRingFile->setText(fileName);
}

void SettingsDialog::on_pbSave_clicked()
{
    ISettings *isettings = new ISettings(this);

//    QDomElement = isettings->getConfigNode("portaudio");

    delete isettings;
}
