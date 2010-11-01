#include "flash_dialog.h"
#include "ui_flash_dialog.h"
#include <qwebsettings.h>
#include <qurl.h>
#include "trainer_studio.h"
#include "server_connection.h"
#include <QWebFrame>
#include <QWebElement>
#include "fs_host.h"
#include "main_window.h"
#include "qmessagebox.h"

FlashDialog::FlashDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::FlashDialog)
{
  // Enable plugins
  QWebSettings *websetting= QWebSettings::globalSettings();
  websetting->setAttribute(QWebSettings::JavascriptEnabled,true);
  websetting->setAttribute(QWebSettings::PluginsEnabled,true);

  ui->setupUi(this);
  setLayout(ui->verticalLayout);
  this->setWindowFlags(Qt::Window);

  _timer = new QTimer(this);
  _timer->setInterval(1000);
  // Signals
  this->connect(_timer, SIGNAL(timeout()), this, SLOT(onTimerTimeout()));
  this->connect(server_connection, SIGNAL(reservedForInteraction(QVariantMap)), this, SLOT(onReservedForInteraction(QVariantMap)));
  this->connect(server_connection, SIGNAL(lostConnection()), this, SLOT(onLostConnection()));
  this->connect(server_connection, SIGNAL(interactionReconnected()), this, SLOT(onInteractionReconnected()));
  this->connect(server_connection, SIGNAL(invokeMessage(QString)), this, SLOT(onInvokeMessage(QString)));
  this->connect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(onLoadFinished(bool)));

  //    ui->webView->load(QUrl("about:blank"));
  // Load a blank HTML to avoid cross domain communication between JS and Flash
  QSettings settings;
  QString url = settings.value("General/url").toString();
  ui->webView->load(QUrl(QString("%1/user/keep_alive").arg(url)));

  // Allow JS call QT callback using the mainWindow object
  ui->webView->page()->mainFrame()->addToJavaScriptWindowObject("mainWindow", this);
  this->connect(ui->webView->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(onJSWindowObjectCleared()));

  // Read JS into memory
  QFile file;
  file.setFileName(":/resources/loadflash.js");
  file.open(QIODevice::ReadOnly);
  _js = file.readAll();
  file.close();
}

FlashDialog::~FlashDialog()
{
  _timer->stop();
  delete _timer;
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

void FlashDialog::showEvent(QShowEvent *e)
{
  qDebug() << e;
  ui->lbTimer->setText("00:00:00");
  _tickCount = 0;
  _timer->start();
}

void FlashDialog::closeEvent(QCloseEvent *e)
{
  //show warning

  // Make sure mic unmuted
  QString res;
  fshost->sendCmd("pa", "flags on mouth", &res);
  fshost->sendCmd("hupall", "", &res);
  ui->webView->reload();
  lower();
  qDebug() << "FlashDialog closing...";
}

void FlashDialog::onTimerTimeout()
{

  QTime t(0,0,0);

  t = t.addSecs(_tickCount++);
  ui->lbTimer->setText(t.toString("hh:mm:ss"));
}

void FlashDialog::onReservedForInteraction(QVariantMap data)
{
  QSettings settings;
  QString url = settings.value("General/url").toString();
  QVariantMap user = ((MainWindow *)(this->parent()))->getUser();
  qDebug() << user["login"];

  _interactionID = data["interaction_id"].toString();


  QString params = QString("var url='%1/flex/interaction/trainer/interaction.swf';"
                           "var vars='realtime_host=%2"
                           "&realtime_channel=%3"
                           "&font_size=12&cs_number=4008871020&realtime_port=%4"
                           "&interaction_id=%5"
                           "&scenario_id=%6"
                           "&realtime_subscriber=%7"
                           "&trainer_login=%8"
                           "&trainer_pwd=%9"
                           "&base_url=%10';"
                          ).arg(url
                               ).arg("127.0.0.1"
                                    ).arg(data["realtime_uuid"].toString()
                                         ).arg("2000"
                                              ).arg(_interactionID
                                                   ).arg(data["scenario_id"].toString()
                                                        ).arg(user["login"].toString()
                                                             ).arg(user["login"].toString()
                                                                  ).arg("test"
                                                                       ).arg(url
                                                                            );
  //Load movie useing js
  loadMovie(params);
  ui->btnReconnect->setStyleSheet("background-color: ;");
  ui->btnReconnect->setText("Reconnect");
  ui->btnReconnect->setEnabled(true);
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
  _timer->stop();

  QSettings settings;
  QString url = settings.value("General/url").toString();

  QString res;
  fshost->sendCmd("pa", "hangup", &res);

  if (_tickCount < 450 ) {
    int ret = QMessageBox::warning(this, "Premature Ending",
                                   "This call was ended prematurely.  Would you like to skip the review screen?",
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (ret == QMessageBox::Yes) {
      ui->webView->reload();
      lower();
      hide();
      return;
    }
  }

  //  FlashVars does't work for this swf, so need to set params in url. Hmmm...

  QString vars = QString("product_type=eqenglish"
                         "&background_color=#F3F3F3"
                         "&font_family=Arial"
                         "&default_ui_language=en_US"
                         "&ui_language=en_US"
                         "&mode=trainer"
                         "&interaction_id=%1"
                         "&base_url=%2"
                        ).arg(_interactionID
                             ).arg(url
                                  );
  QString params = QString("var url='%1/flex/markspot/markspot.swf?%2';"
                           "var vars='%2';").arg(url).arg(vars);
  loadMovie(params);
  server_connection->review();

  _tickCount = 0; //reset for review
  _timer->start();
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
    //        ui->webView->load(QUrl("http://www.eqenglish.com/user/keep_alive"));
  } else if (f == 1) {
    f++;
    //        ui->webView->load(QUrl("http://www.eqenglish.com/flex/interaction/trainer/interaction.swf?realtime_port=2000&trainer_login=trainer28&trainer_pwd=test&interaction_id=106357&font_size=12&realtime_subscriber=trainer28&environment=production&cs_number=400-887-1020&realtime_channel=a3379aba14f3da5caa6a2760a06e336e8c7c9bac&base_url=http://www.eqenglish.com&realtime_host=127.0.0.1&scenario_id=697"));
    QString jsLoadFlash=QString("var vars='realtime_port=2000&trainer_login=trainer28&trainer_pwd=test&interaction_id=106357&font_size=12&realtime_subscriber=trainer28&environment=production&cs_number=400-887-1020&realtime_channel=a3379aba14f3da5caa6a2760a06e336e8c7c9bac&base_url=http://www.eqenglish.com&realtime_host=127.0.0.1&scenario_id=697';"
                                "var url='http://www.eqenglish.com/flex/interaction/trainer/interaction.swf';%1").arg(_js);
    qDebug() << jsLoadFlash;
    ui->webView->page()->mainFrame()->evaluateJavaScript(jsLoadFlash);
  } else if( f==2){
    f=0;

    QString s = "var url='http://www.eqenglish.com/flex/markspot/markspot.swf?product_type=eqenglish&background_color=#F3F3F3&font_family=Arial&default_ui_language=en_US&ui_language=en_US&mode=trainer&interaction_id=107261&base_url=http://www.eqenglish.com';"
     "var vars='product_type=eqenglish&background_color=#F3F3F3&font_family=Arial&default_ui_language=en_US&ui_language=en_US&mode=trainer&interaction_id=107261&base_url=http://www.eqenglish.com';"
     ;

    QString ss = QString("%1;%2").arg(s).arg(_js);
    ui->webView->page()->mainFrame()->evaluateJavaScript(ss);

    //        QWebFrame *frame = ui->webView->page()->mainFrame();
    //        frame->addToJavaScriptWindowObject("mainWindow", this);
    //        QWebElement e = frame->findFirstElement("embed");
    //
    //        QString s="this.FlashVars='product_type=eqenglish&background_color=#F3F3F3&font_family=Arial&default_ui_language=en_US&ui_language=en_US&mode=trainer&interaction_id=107261&base_url=http://www.eqenglish.com';"
    //                  "alert(this);"
    //                  "this.LoadMovie(0, 'http://www.eqenglish.com/flex/markspot/markspot.swf?product_type=eqenglish&background_color=#F3F3F3&font_family=Arial&default_ui_language=en_US&ui_language=en_US&mode=trainer&interaction_id=108219&base_url=http://www.eqenglish.com');";
    //        qDebug() << s;
    //        e.evaluateJavaScript(s);

  }
}

// Called from flash (the *finish* button)
void FlashDialog::onFSCommand(QString cmd, QString args)
{
  qDebug() << "--------FSCommand:\n\n" << cmd << ": " << args;
  if(cmd == "saved" || cmd == "committedProblems"){
    _timer->stop();
    // reset webview
    ui->webView->reload();
    lower();
    hide();
  } else if(cmd == "log") {
    qDebug() << "Flash Log: " << args;
  } else {
    qDebug() << "Unknown FS command: " << cmd;
  }
}


void FlashDialog::on_btnReconnect_clicked()
{
  int ret = QMessageBox::warning(this, "TS",
                                 "This will attempt to re-connect you to the student and fix sound issues."
                                 "The student will be disconnected and re-called."
                                 "Please do not hit this button twice."
                                 "This re-connect may take a few seconds."
                                 "Are you sure you want to continue?",
                                 QMessageBox::Yes | QMessageBox::No );
  if (ret == QMessageBox::No) return;

  server_connection->startInteractionReconnection(_interactionID);
  onLostConnection();
}

void FlashDialog::onLostConnection()
{
  _timer->stop();

  ui->btnReconnect->setStyleSheet("background-color: red;");
  ui->btnReconnect->setText("Reconnecting...");
  ui->btnReconnect->setEnabled(false);
}

void FlashDialog::onInteractionReconnected()
{
  _timer->start();

  qDebug() << "onReconnected";
  ui->btnReconnect->setStyleSheet("background-color: ;");
  ui->btnReconnect->setText("Reconnect");
  ui->btnReconnect->setEnabled(true);
}

void FlashDialog::onInvokeMessage(QString msg)
{
  qDebug() << "Invoke msg: " << msg;
}

void FlashDialog::loadMovie(QString params)
{
  qDebug() << params;
  ui->webView->page()->mainFrame()->evaluateJavaScript(params + _js);
}

void FlashDialog::onJSWindowObjectCleared()
{
  qDebug() << "Object Cleared!!!!";

  ui->webView->page()->mainFrame()->addToJavaScriptWindowObject("mainWindow", this);
}

void FlashDialog::on_tbMute_clicked()
{
  QString res;
  if (ui->tbMute->text() == "Mute") {
    fshost->sendCmd("pa", "flags off mouth", &res);
    ui->tbMute->setText("UnMute");
  } else {
    fshost->sendCmd("pa", "flags on mouth", &res);
    ui->tbMute->setText("Mute");
  }
  qDebug() << res;
}
