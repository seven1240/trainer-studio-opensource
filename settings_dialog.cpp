#include <QtXml>
#include <QFileDialog.h>
#include <QMessageBox.h>
#include "settings_dialog.h"
#include "ui_settings_dialog.h"
#include "trainer_studio.h"
#include "fs_host.h"
#include "isettings.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SettingsDialog)
{
  ui->setupUi(this);
  ui->cbTemplate->addItem("EQEnglish Trainer");
  ui->cbTemplate->addItem("EQEnglish Trainer (China)");
  ui->cbTemplate->addItem("Test Server");
  ui->cbTemplate->addItem("Developer");
  ui->cbTemplate->setCurrentIndex(-1);

  QSettings settings;
  settings.beginGroup("General");
  ui->leURL->setText(settings.value("url").toString());
  ui->leServer->setText(settings.value("trainer_server").toString() +
                        ":" + settings.value("trainer_server_port").toString());
  if (settings.value("sip_transport").toString() == "tcp") {
    ui->cbSIPTransport->setChecked(true);
  }
  settings.endGroup();

  ignore_change_event = true;
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
    ui->leURL->setText("http://www.idapted.com");
    ui->leServer->setText("voip.idapted.com:7000");
    break;
  case 1:
    ui->leURL->setText("http://www.eqenglish.com");
    ui->leServer->setText("voip.idapted.com:7000");
    break;
  case 2:
    ui->leURL->setText("http://www.veecue.com");
    ui->leServer->setText("voip.veecue.com:7000");
    break;
  case 3:
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
  settings.setValue("trainer_server_port", slServer.at(1) == "" ? 7000 : slServer.at(1).toInt() );
  settings.setValue("sip_transport", ui->cbSIPTransport->isChecked() ? "tcp" : "auto");
  settings.endGroup();
  QDateTime t = QDateTime::currentDateTime();
  ui->lbMessage->setText("Saved at " + t.toString("yyyy-MM-dd hh:mm:ss"));
  ui->lbMessage->setStyleSheet("color:red");
}

void SettingsDialog::updateDevlist()
{
  QString devices;

  ignore_change_event = true;

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
  ui->cbInput->addItem("Default Device", "");
  ui->cbOutput->addItem("Default Device", "");
  ui->cbRing->addItem("Default Device", "");

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
          ui->cbRing->setCurrentIndex(ui->cbRing->count() -1);
        }
        if ("o" == flaglist.at(0) || "o" == flaglist.at(1)) {
          ui->cbOutput->setCurrentIndex(ui->cbOutput->count() -1);
        }
        if ("i" == flaglist.at(0) || "i" == flaglist.at(1)) {
          ui->cbInput->setCurrentIndex(ui->cbInput->count() -1);
        }
      } else {
        if("r" == flag) {
          ui->cbRing->setCurrentIndex(ui->cbRing->count() -1);
        } else if("o" == flag) {
          ui->cbOutput->setCurrentIndex(ui->cbOutput->count() -1);
        } else if("i" == flag) {
          ui->cbInput->setCurrentIndex(ui->cbInput->count() -1);
        }
      }
    }
  }

  ISettings *isettings = new ISettings(this);
  ui->leRingFile->setText(isettings->getPaRingFile());
  delete isettings;
  ignore_change_event = false;
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
  if(!fileName.isEmpty()) ui->leRingFile->setText(fileName);
}

void SettingsDialog::on_pbSave_clicked()
{
  QString indev = ui->cbInput->itemData(ui->cbInput->currentIndex()).toString();
  QString outdev = ui->cbOutput->itemData(ui->cbOutput->currentIndex()).toString();
  QString ringdev = ui->cbRing->itemData(ui->cbRing->currentIndex()).toString();

  QVariantMap conf;
  conf.insert("ring-file", ui->leRingFile->text());
  conf.insert("indev", indev);
  conf.insert("outdev", outdev);
  conf.insert("ringdev", ringdev);

  ISettings *isettings = new ISettings(this);
  isettings->writePaConfig(conf);
  isettings->saveToFile();
  delete isettings;

  QMessageBox::information(this, "OK", "saved ok");
}

void SettingsDialog::on_cbInput_currentIndexChanged(int index)
{
  QString res;
  if(ignore_change_event) return;
  fshost->sendCmd("pa", QString("indev #%1").arg(index).toAscii(), &res);
}

void SettingsDialog::on_cbOutput_currentIndexChanged(int index)
{
  QString res;
  if(ignore_change_event) return;
  fshost->sendCmd("pa", QString("outdev #%1").arg(index).toAscii(), &res);
}

void SettingsDialog::on_cbRing_currentIndexChanged(int index)
{
  QString res;
  if(ignore_change_event) return;
  fshost->sendCmd("pa", QString("ringdev #%1").arg(index).toAscii(), &res);
}

void SettingsDialog::setActiveTabs(unsigned int tabs)
{
  for(int i=0; i<ui->tabWidget->count(); i++) {
    if (tabs & (1<< i)) {
      ui->tabWidget->setTabEnabled(i, true);
    } else {
      ui->tabWidget->setTabEnabled(i, false);
    }
  }
}
