#include "flashdialog.h"
#include "ui_flashdialog.h"
#include <qwebsettings.h>
#include <qurl.h>
#include "trainer_studio.h"
#include "TCPClient.h"
#include <QWebFrame>
#include <QWebElement>
#include "fshost.h"
#include "mainwindow.h"


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
    QSettings settings;
    QString url = settings.value("General/url").toString();
    QVariantMap user = ((MainWindow *)(this->parent()))->getUser();
    qDebug() << user["login"];

    interactionID = data["interaction_id"].toString();


    QString jsLoadFlash = QString("var url='%1/flex/interaction/trainer/interaction.swf';"
                                 "var vars='realtime_host=%2"
                                 "&realtime_channel=%3"
                                 "&font_size=12&cs_number=4008871020&realtime_port=%4"
                                 "&interaction_id=%5"
                                 "&scenario_id=%6"
                                 "&realtime_subscriber=%7"
                                 "&trainer_login=%8"
                                 "&trainer_pwd=%9"
                                 "&base_url=%10';%11"
                                 ).arg(url
                                 ).arg("127.0.0.1"
                                 ).arg(data["realtime_uuid"].toString()
                                 ).arg("2000"
                                 ).arg(interactionID
                                 ).arg(data["scenario_id"].toString()
                                 ).arg(user["login"].toString()
                                 ).arg(user["login"].toString()
                                 ).arg("test"
                                 ).arg(url
                                 ).arg(js
                                 );

    qDebug() << jsLoadFlash;
    ui->webView->reload();
    ui->webView->page()->mainFrame()->evaluateJavaScript(jsLoadFlash);
}

void FlashDialog::onLoadFinished(bool)
{
    return;
    QUrl url = ui->webView->url();
    if(url.toString().indexOf("/markspot.swf") < 0) {
        return;
    }
    qDebug() << "LoadFinished";
    //    QString flash_vars = QString("realtime_host=%1&realtime_channel=%2&realtime_uuid=%3&font_size=12&cs_number=4008871020&realtime_port=2000&interaction_id=%4&scenario_id=%5&realtime_subscriber=%6&trainer_login=%7&trainer_pwd=%8&")

    QWebFrame *frame = ui->webView->page()->mainFrame();
    frame->addToJavaScriptWindowObject("mainWindow", this);
    QWebElement e = frame->findFirstElement("embed");
//    QWebElementCollection c = frame->findAllElements("*");
//    for(int i=0; i<c.count(); i++) {
//        qDebug() << c.at(i).tagName() << ": " << c.at(i).toInnerXml();
//    }

    QString jsstr = "mainWindow.onFSCommand('a', 'b'); function plugin_DoFSCommand(cmd, args){alert(cmd); alert(args);} alert(this.name);";
    e.evaluateJavaScript(jsstr);
    qDebug() << e.toInnerXml();
    qDebug() << ui->webView->url();
//    e.evaluateJavaScript("alert(this);this.FlashVars='aaaa=b';this.LoadMovie(0, 'http://www.eqenglish.com/flex/interaction/trainer/interaction.swf');");
}

void FlashDialog::on_btnDisconnect_clicked()
{
    QSettings settings;
    QString url = settings.value("General/url").toString();

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
                                     ).arg(url
                                     ).arg(interactionID
                                     ).arg(url
                                     );
    ui->webView->load(QUrl(flash_url));
}

void FlashDialog::on_btnTest_clicked()
{
//    QString vars = "var url='http://www.eqenglish.com/flex/interaction/trainer/interaction.swf';var vars='realtime_port=2000&trainer_login=trainer28&interaction_id=106357&font_size=12&realtime_subscriber=trainer28&environment=production&cs_number=400-887-1020&realtime_channel=a3379aba14f3da5caa6a2760a06e336e8c7c9bac&base_url=http://www.eqenglish.com&realtime_host=10.20.13.227&scenario_id=697';";
//    QString vars = "var url='http://www.eqenglish.com/flex/markspot/markspot.swf';var vars='product_type=eqenglish&background_color=#F3F3F3&font_family=Arial&default_ui_language=en_US&ui_language=en_US&mode=trainer&interaction_id=107261&base_url=http://www.eqenglish.com';";
//    QString vars = "var url='http://localhost:8000/markspot.swf';var vars='product_type=eqenglish';";
//    QString  js1 = vars + js;
//    qDebug() << js1;
//    ui->webView->reload();
//    ui->webView->page()->mainFrame()->evaluateJavaScript(js1);
//    ui->webView->load(QUrl("http://localhost:8000/markspot.swf?product_type=eqenglish"));
    static int f = 0;

    if(f == 0) {
        f++;
        ui->webView->load(QUrl("http://www.eqenglish.com/user/keep_alive"));
    } else if (f == 1) {
        f++;
//        ui->webView->load(QUrl("http://www.eqenglish.com/flex/interaction/trainer/interaction.swf?realtime_port=2000&trainer_login=trainer28&trainer_pwd=test&interaction_id=106357&font_size=12&realtime_subscriber=trainer28&environment=production&cs_number=400-887-1020&realtime_channel=a3379aba14f3da5caa6a2760a06e336e8c7c9bac&base_url=http://www.eqenglish.com&realtime_host=127.0.0.1&scenario_id=697"));
        QString jsLoadFlash=QString("var vars='realtime_port=2000&trainer_login=trainer28&trainer_pwd=test&interaction_id=106357&font_size=12&realtime_subscriber=trainer28&environment=production&cs_number=400-887-1020&realtime_channel=a3379aba14f3da5caa6a2760a06e336e8c7c9bac&base_url=http://www.eqenglish.com&realtime_host=127.0.0.1&scenario_id=697';"
                            "var url='http://www.eqenglish.com/flex/interaction/trainer/interaction.swf';%1").arg(js);
        qDebug() << jsLoadFlash;
        ui->webView->page()->mainFrame()->evaluateJavaScript(jsLoadFlash);
    } else if( f==2){
        f=0;

//        ui->webView->load(QUrl("http://www.eqenglish.com/flex/markspot/markspot.swf?product_type=eqenglish&background_color=#F3F3F3&font_family=Arial&default_ui_language=en_US&ui_language=en_US&mode=trainer&interaction_id=107261&base_url=http://www.eqenglish.com"));

        QWebFrame *frame = ui->webView->page()->mainFrame();
        frame->addToJavaScriptWindowObject("mainWindow", this);
        QWebElement e = frame->findFirstElement("embed");

        QString s="this.FlashVars='product_type=eqenglish&background_color=#F3F3F3&font_family=Arial&default_ui_language=en_US&ui_language=en_US&mode=trainer&interaction_id=107261&base_url=http://www.eqenglish.com';"
                  "alert(this);"
                  "this.LoadMovie(0, 'http://www.eqenglish.com/flex/markspot/markspot.swf?product_type=eqenglish&background_color=#F3F3F3&font_family=Arial&default_ui_language=en_US&ui_language=en_US&mode=trainer&interaction_id=108219&base_url=http://www.eqenglish.com');";
        qDebug() << s;
        e.evaluateJavaScript("alert(movie_DoFSCommand);");
        e.evaluateJavaScript(s);

    }
}

void FlashDialog::onFSCommand(QString cmd, QString args)
{
    qDebug() << "--------FSCommand:\n\n" << cmd << ": " << args;
    if(cmd == "saved" || cmd == "committedProblems"){
        //finished = True
        ui->webView->load(QUrl("about:blank"));
        hide();
    } else if(cmd == "log") {
        qDebug() << "LOG: " << args;
    }
}

void FlashDialog::on_btnFinish_clicked()
{
    qDebug() << "Nothing";
}
