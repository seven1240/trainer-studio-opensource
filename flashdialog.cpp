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
//    settings->setAttribute(QWebSettings::PluginsEnabled, true);

    this->connect(tcp_client, SIGNAL(reservedForInteraction(QVariantMap)), this, SLOT(onReservedForInteraction(QVariantMap)));
    this->connect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(onLoadFinished(bool)));


    ui->webView->load(QUrl("about:blank"));

    qDebug() << (settings->testAttribute(QWebSettings::PluginsEnabled) ? " true" : " false");

    QFile file;
    file.setFileName(":/resources/loadflash.js");
    file.open(QIODevice::ReadOnly);
    js = file.readAll();
    file.close();

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
    interactionID = data["interaction_id"].toString();

    QString jsLoadFlash = QString("var url='%1/flex/interaction/trainer/interaction.swf';"
                                 "var vars='realtime_host=%2"
                                 "&realtime_channel=%3"
                                 "&font_size=12&cs_number=4008871020&realtime_port=%4"
                                 "&interaction_id=%5"
                                 "&scenario_id=%6"
                                 "&realtime_subscriber=%7"
                                 "&trainer_login=%8"
                                 "&trainer_xxxxpwd=%9"
                                 "&base_url=%10';%11"
                                 ).arg("http://www.eqenglish.com"
                                 ).arg("127.0.0.1"
                                 ).arg(data["realtime_uuid"].toString()
                                 ).arg("2000"
                                 ).arg(interactionID
                                 ).arg(data["scenario_id"].toString()
                                 ).arg("trainer28"
                                 ).arg("trainer28"
                                 ).arg("test"
                                 ).arg("http://www.eqenglish.com"
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
//    e.evaluateJavaScript("alert(this);this.FlashVars='aaaa=b';this.LoadMovie(0, 'http://www.eqenglish.com/flex/interaction/trainer/interaction.swf');");
}

void FlashDialog::on_btnDisconnect_clicked()
{
    QString res;
    fshost->sendCmd("pa", "hangup", &res);

//  FlashVars does't work for this swf, Hmmm...

    QString flash_url = QString("%1/flex/markspot/markspot.swf?"
                                     "product_type=eqenglish"
                                     "&background_color=#F3F3F3"
                                     "&font_family=Arial"
                                     "&default_ui_language=en_US"
                                     "&ui_language=en_US"
                                     "&mode=trainer"
                                     "&interaction_id=%2"
                                     "&base_url=%3"
                                     ).arg("http://www.eqenglish.com"
                                     ).arg(interactionID
                                     ).arg("http://www.eqenglish.com"
                                     );
    ui->webView->load(QUrl(flash_url));
}

void FlashDialog::on_btnTest_clicked()
{
    QString vars = "var url='http://www.eqenglish.com/flex/interaction/trainer/interaction.swf';var vars='realtime_port=2000&trainer_login=trainer28&interaction_id=106357&font_size=12&realtime_subscriber=trainer28&environment=production&cs_number=400-887-1020&realtime_channel=a3379aba14f3da5caa6a2760a06e336e8c7c9bac&base_url=http://www.eqenglish.com&realtime_host=10.20.13.227&scenario_id=697';";
//    QString vars = "var url='http://www.eqenglish.com/flex/markspot/markspot.swf';var vars='product_type=eqenglish&background_color=#F3F3F3&font_family=Arial&default_ui_language=en_US&ui_language=en_US&mode=trainer&interaction_id=107261&base_url=http://www.eqenglish.com';";
//    QString vars = "var url='http://localhost:8000/markspot.swf';var vars='product_type=eqenglish';";
//    QString  js1 = vars + js;
//    qDebug() << js1;
//    ui->webView->reload();
//    ui->webView->page()->mainFrame()->evaluateJavaScript(js1);
//    ui->webView->load(QUrl("http://localhost:8000/markspot.swf?product_type=eqenglish"));
    ui->webView->load(QUrl("http://www.eqenglish.com/flex/interaction/trainer/interaction.swf?realtime_port=2000&trainer_login=trainer28&interaction_id=106357&font_size=12&realtime_subscriber=trainer28&environment=production&cs_number=400-887-1020&realtime_channel=a3379aba14f3da5caa6a2760a06e336e8c7c9bac&base_url=http://www.eqenglish.com&realtime_host=127.0.0.1&scenario_id=697"));
}
