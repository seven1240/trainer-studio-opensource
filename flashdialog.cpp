#include "flashdialog.h"
#include "ui_flashdialog.h"
#include "qwebsettings.h"
#include "qdebug.h"
#include "qurl.h"


FlashDialog::FlashDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FlashDialog)
{
    QWebSettings *websetting= QWebSettings::globalSettings();
      websetting->setAttribute(QWebSettings::JavascriptEnabled,true);
      websetting->setAttribute(QWebSettings::PluginsEnabled,true);

    ui->setupUi(this);
    setLayout(ui->verticalLayout);

    QWebSettings *settings = ui->webView->settings();

//    settings->setAttribute(QWebSettings::PluginsEnabled, true);

//    ui->webView->load(QUrl("http://www.adobe.com/software/flash/about/"));
    ui->webView->load(QUrl("http://www.idapted.com"));
//    ui->webView->load(QUrl("http://www.communitymx.com/content/source/E5141/wmodetrans.htm"));

    qDebug() << (settings->testAttribute(QWebSettings::PluginsEnabled) ? " true" : " false");
}

FlashDialog::~FlashDialog()
{
    delete ui;
}

void FlashDialog::changeEvent(QEvent *e)
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
