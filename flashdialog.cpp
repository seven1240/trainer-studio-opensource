#include "flashdialog.h"
#include "ui_flashdialog.h"
#include <qwebsettings.h>
#include <qurl.h>
#include "trainer_studio.h"
#include "TCPClient.h"
#include <QWebFrame>
#include <QWebElement>
#include "fshost.h"


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

    this->connect(tcp_client, SIGNAL(reservedForInteraction(QVariantMap)), this, SLOT(onReservedForInteraction(QVariantMap)));
    this->connect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(onLoadFinished(bool)));
//    settings->setAttribute(QWebSettings::PluginsEnabled, true);

//    ui->webView->load(QUrl("http://www.adobe.com/software/flash/about/"));
//    ui->webView->load(QUrl("http://www.idapted.com/flex/interaction/trainer/interaction.swf"));
//    ui->webView->load(QUrl("http://www.idapted.com/images/banner.swf"));
    ui->webView->load(QUrl("about:blank"));

    qDebug() << (settings->testAttribute(QWebSettings::PluginsEnabled) ? " true" : " false");

    QFile file;
    file.setFileName(":/resources/loadflash.js");
    file.open(QIODevice::ReadOnly);
    js = file.readAll();
    file.close();

//    QString vars="var vars='a=b';";
//    vars = "var vars='realtime_port=2000&trainer_login=lpatrick281&interaction_id=106344&font_size=12&realtime_subscriber=lpatrick281&environment=production&cs_number=400-887-1020&realtime_channel=4a5599957289d1e9456f0c334fc0dbdbe0085b75&base_url=http://www.eqenglish.com&realtime_host=10.20.13.227&scenario_id=837';";
//    js = vars + js;
//    qDebug() << js;
//    ui->webView->page()->mainFrame()->evaluateJavaScript(js);
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

void FlashDialog::onReservedForInteraction(QVariantMap data)
{
    QString jsLoadFlash = QString("var vars='realtime_host=%1"
                                 "&realtime_channel=%2"
                                 "&font_size=12&cs_number=4008871020&realtime_port=%3"
                                 "&interaction_id=%4"
                                 "&scenario_id=%5"
                                 "&realtime_subscriber=%6"
                                 "&trainer_login=%7"
                                 "&trainer_xxxxpwd=%8"
                                 "&base_url=%9';%10"
                                 ).arg("127.0.0.1"
                                 ).arg(data["realtime_uuid"].toString()
                                 ).arg("2000"
                                 ).arg(data["interaction_id"].toString()
                                 ).arg(data["scenario_id"].toString()
                                 ).arg("trainer28"
                                 ).arg("trainer28"
                                 ).arg("test"
                                 ).arg("http://www.idapted.com"
                                 ).arg(js
                                 );

    qDebug() << jsLoadFlash;
    ui->webView->reload();
    ui->webView->page()->mainFrame()->evaluateJavaScript(jsLoadFlash);
}

void FlashDialog::onLoadFinished(bool)
{

    qDebug() << "LoadFinished";
    //    QString flash_vars = QString("realtime_host=%1&realtime_channel=%2&realtime_uuid=%3&font_size=12&cs_number=4008871020&realtime_port=2000&interaction_id=%4&scenario_id=%5&realtime_subscriber=%6&trainer_login=%7&trainer_pwd=%8&")

//    QWebFrame *frame = ui->webView->page()->mainFrame();
//    QWebElement e = frame->findFirstElement("embed");
//    qDebug() << e.toInnerXml();
//    e.evaluateJavaScript("alert(this);this.FlashVars='aaaa=b';this.LoadMovie(0, 'http://www.idapted.com/flex/interaction/trainer/interaction.swf');");
}

void FlashDialog::on_btnDisconnect_clicked()
{
    QString res;
    fshost->sendCmd("pa", "hangup", &res);
}
