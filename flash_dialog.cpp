#include <QWebSettings.h>
#include <QMessageBox>
#include <QWebView>
#include <QWebFrame>
#include <QWebElement>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QUrl.h>
#include <QApplication>
#include <QClipboard>
#include "trainer_studio.h"
#include "application_controller.h"
#include "flash_controller.h"
#include "server_connection.h"
#include "freeswitch.h"
#include "flash_dialog.h"
#include "main_window.h"
#include "utils.h"

#define REVIEW_FLASH_URL "/trainer/review/flex"

FlashDialog::FlashDialog(QWidget *parent) :
	QDialog(parent)
{
	_webView = new QWebView();
	_hangup = new QPushButton("Hangup");
	_reconnection = new QPushButton("Reconnect");
	_mute = new QPushButton("Mute");
	_mute->setCheckable(true);
	_test = new QPushButton("Test");
	_reloadMovie = new QPushButton("Reload");
	_time = new QLabel();

	QPushButton *copyInteractionId = new QPushButton("Copy IID");

	QHBoxLayout *topLayout = new QHBoxLayout();
	QFrame *topFrame = new QFrame();
	topLayout->addWidget(_time);
	topLayout->addWidget(_mute);
	topLayout->addWidget(_test);
	topLayout->addWidget(_reloadMovie);
	topLayout->addWidget(copyInteractionId);
	topLayout->addWidget(_hangup);
	topLayout->addWidget(_reconnection);
	topFrame->setLayout(topLayout);
	topFrame->setFixedHeight(40);

	QVBoxLayout *verticalLayout = new QVBoxLayout();
	verticalLayout->addWidget(topFrame);
	verticalLayout->addWidget(_webView);
	setLayout(verticalLayout);

	resize(1024, 728);
	Utils::centerWindowOnDesktop(this);

	_timer = new QTimer(this);
	_timer->setInterval(1000);

	connect(_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
	connect(ApplicationController::server(), SIGNAL(reservedForInteraction(QVariantMap)), this, SLOT(onReservedForInteraction(QVariantMap)));
	connect(ApplicationController::server(), SIGNAL(lostConnection()), this, SLOT(onLostConnection()));
	connect(ApplicationController::server(), SIGNAL(interactionReconnected()), this, SLOT(onInteractionReconnected()));
	connect(ApplicationController::server(), SIGNAL(invokeMessage(QString)), this, SLOT(onInvokeMessage(QString)));
	connect(ApplicationController::fs(), SIGNAL(callEnded(QString,QString,QString)), this, SLOT(onCallHangup(QString,QString,QString)));
	connect(_webView, SIGNAL(loadFinished(bool)), this, SLOT(onLoadFinished(bool)));

	connect(_hangup, SIGNAL(clicked()), this, SLOT(onHangupClicked()));
	connect(_reconnection, SIGNAL(clicked()), this, SLOT(onReconnectionClicked()));
	connect(_mute, SIGNAL(clicked()), this, SLOT(onMuteClicked()));
	connect(_test, SIGNAL(clicked()), this, SLOT(onTestClicked()));
	connect(_reloadMovie, SIGNAL(clicked()), this, SLOT(onReloadMovieClicked()));
	connect(copyInteractionId, SIGNAL(clicked()), this, SLOT(copyInteractionId()));

	QWebSettings *websetting= QWebSettings::globalSettings();
	websetting->setAttribute(QWebSettings::JavascriptEnabled, true);
	websetting->setAttribute(QWebSettings::PluginsEnabled, true);

	// Load a blank HTML to avoid cross domain communication between JS and Flash
	QSettings settings;
	QString url = settings.value("General/url").toString();
	_webView->load(QUrl(QString("%1/user/keep_alive").arg(url)));

	// Allow JS call QT callback using the mainWindow object
	_webView->page()->mainFrame()->addToJavaScriptWindowObject("mainWindow", this);
	connect(_webView->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(onJSWindowObjectCleared()));

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
}

void FlashDialog::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		// ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void FlashDialog::showEvent(QShowEvent * /*e*/)
{
	_test->setVisible(ApplicationController::isDebugging());
	_seconds = 0;
	onTimer();
	_timer->start();
}

void FlashDialog::closeEvent(QCloseEvent * /*e*/)
{
	ApplicationController::fs()->unmute();
	ApplicationController::fs()->hangup(true);
	_webView->reload();
	lower();
	qDebug() << "flashDialog Closing";
	emit closed();
}

void FlashDialog::onTimer()
{
	QTime time(0, 0, 0);
	time = time.addSecs(_seconds++);
	_time->setText(time.toString("hh:mm:ss"));
}

void FlashDialog::onReservedForInteraction(QVariantMap data)
{
	_currentMovie = MOVIE_INTERACTION;
	_interactionData = data;
	loadInteractionMovie();
}

void FlashDialog::loadInteractionMovie()
{
	QSettings settings;
	QString url = settings.value("General/url").toString();
	User *user = ApplicationController::user();
	QVariantMap data = _interactionData;

	_interactionId = data["interaction_id"].toString();

	QString params = QString("var url='%1/flex/interaction/trainer/interaction.swf';"
							 "var vars='realtime_host=%2"
							 "&realtime_channel=%3"
							 "&font_size=12&cs_number=4008871020&realtime_port=%4"
							 "&interaction_id=%5"
							 "&scenario_id=%6"
							 "&realtime_subscriber=%7"
							 "&trainer_login=%8"
							 "&trainer_pwd=%9"
							 "&base_url=%10';").
	 arg(url).
	 arg("127.0.0.1").
	 arg(data["realtime_uuid"].toString()).
	 arg("2000").
	 arg(_interactionId).
	 arg(data["scenario_id"].toString()).
	 arg(user->getLogin()).
	 arg(user->getLogin()).
	 arg("test").
	 arg(url);

	loadMovie(params);
	reconnectingStatus(false);
}

void FlashDialog::onLoadFinished(bool)
{
	qDebug() << "Webview Load Finished";
	return;
	QUrl url = _webView->url();
	if (url.toString().indexOf("/markspot.swf") < 0) {
		return;
	}
	//    QString flash_vars = QString("realtime_host=%1&realtime_channel=%2&realtime_uuid=%3&font_size=12&cs_number=4008871020&realtime_port=2000&interaction_id=%4&scenario_id=%5&realtime_subscriber=%6&trainer_login=%7&trainer_pwd=%8&")

	QWebFrame *frame = _webView->page()->mainFrame();
	frame->addToJavaScriptWindowObject("mainWindow", this);
	QWebElement e = frame->findFirstElement("embed");
	//    QWebElementCollection c = frame->findAllElements("*");
	//    for(int i=0; i<c.count(); i++) {
	//        qDebug() << c.at(i).tagName() << ": " << c.at(i).toInnerXml();
	//    }

	QString jsstr = "mainWindow.onFSCommand('a', 'b'); function plugin_DoFSCommand(cmd, args){alert(cmd); alert(args);} alert(this.name);";
	e.evaluateJavaScript(jsstr);
	qDebug() << e.toInnerXml();
	qDebug() << _webView->url();
	//    e.evaluateJavaScript("alert(this);this.FlashVars='aaaa=b';this.LoadMovie(0, 'http://www.eqenglish.com/flex/interaction/trainer/interaction.swf');");
}

void FlashDialog::onHangupClicked()
{
	_timer->stop();
	ApplicationController::fs()->hangup();
}

void FlashDialog::onCallHangup(QString /*uuid*/, QString /*cidName*/, QString /*cidNumber*/)
{
	if (!this->isVisible()) return;

	if (_seconds < 450) {
		int ret = QMessageBox::warning(this, "Premature Ending",
									   "This call was ended prematurely.  Would you like to skip the review screen?",
									   QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
		if (ret == QMessageBox::Yes) {
			_webView->reload();
			close();
			return;
		}
	}

	_currentMovie = MOVIE_REVIEW;
	ApplicationController::server()->review();
	loadReviewMovie();
}

void FlashDialog::loadReviewMovie()
{
	QSettings settings;
	QString url = settings.value("General/url").toString();

	//  FlashVars does't work for this swf, so need to set params in url. Hmmm...
	QString vars = QString("product_type=eqenglish"
						   "&background_color=%23F3F3F3"
						   "&font_family=Arial"
						   "&default_ui_language=en_US"
						   "&ui_language=en_US"
						   "&mode=trainer"
						   "&interaction_id=%1"
						   "&base_url=%2"
						  ).arg(_interactionId).arg(url);
	QString params = QString("var url='%1/flex/markspot/markspot.swf?%2';var vars='%2';").arg(url).arg(vars);

	QString loadUrl = QString("%1" REVIEW_FLASH_URL "?%2").arg(url).arg(vars);
	_webView->load(QUrl(loadUrl));

//	loadMovie(params);

	_seconds = 0;
	_timer->start();
}

void FlashDialog::onTestClicked()
{
	QSettings settings;
	QString url = settings.value("General/url").toString();
	//    QString vars = "var url='http://www.eqenglish.com/flex/interaction/trainer/interaction.swf';var vars='realtime_port=2000&trainer_login=trainer28&interaction_id=106357&font_size=12&realtime_subscriber=trainer28&environment=production&cs_number=400-887-1020&realtime_channel=a3379aba14f3da5caa6a2760a06e336e8c7c9bac&base_url=http://www.eqenglish.com&realtime_host=10.20.13.227&scenario_id=697';";
	//    QString vars = "var url='http://www.eqenglish.com/flex/markspot/markspot.swf';var vars='product_type=eqenglish&background_color=#F3F3F3&font_family=Arial&default_ui_language=en_US&ui_language=en_US&mode=trainer&interaction_id=107261&base_url=http://www.eqenglish.com';";
	//    QString vars = "var url='http://localhost:8000/markspot.swf';var vars='product_type=eqenglish';";
	//    QString  js1 = vars + js;
	//    qDebug() << js1;
	//    ui->webView->reload();
	//    ui->webView->page()->mainFrame()->evaluateJavaScript(js1);
	//    ui->webView->load(QUrl("http://localhost:8000/markspot.swf?product_type=eqenglish"));
	static int f = 0;

	if (f == 0) {
		f++;
		_webView->load(QUrl(QString("%1/user/keep_alive").arg(url)));
	}
	else if (f == 1) {
		f++;
		QString jsLoadFlash=QString("var vars='realtime_port=2000&trainer_login=trainer28&trainer_pwd=test&interaction_id=106357&font_size=12&realtime_subscriber=trainer28&environment=production&cs_number=400-887-1020&realtime_channel=a3379aba14f3da5caa6a2760a06e336e8c7c9bac&base_url=http://www.eqenglish.com&realtime_host=127.0.0.1&scenario_id=697';"
			"var url='%1/flex/interaction/trainer/interaction.swf';%2").arg(url).arg(_js);
		qDebug() << jsLoadFlash;
		_webView->page()->mainFrame()->evaluateJavaScript(jsLoadFlash);
	}
	else if (f == 2) {
		f=0;
		QString s = QString("%1" REVIEW_FLASH_URL
			"?product_type=eqenglish&background_color=#F3F3F3&font_family=Arial&default_ui_language=en_US"
			"&ui_language=en_US&mode=trainer&interaction_id=108219&base_url=%s").arg(url);
		_webView->load(QUrl(s));
	}
}

void FlashDialog::onReloadMovieClicked()
{
	if (MOVIE_INTERACTION == _currentMovie) {
		onReservedForInteraction(_interactionData);
	} else if (MOVIE_REVIEW == _currentMovie) {
		loadReviewMovie();
	}
}

void FlashDialog::onFSCommand(QString cmd, QString args)
{
	qDebug() << "FSCommand:" << cmd << ": " << args;
	if (cmd == "saved" || cmd == "committedProblems"){
		_timer->stop();
		_webView->reload();
		close();
	}
	else if (cmd == "log") {
		qDebug() << "Flash Log: " << args;
	}
	else {
		qDebug() << "Unknown FS command: " << cmd;
	}
}

void FlashDialog::onReconnectionClicked()
{
	QString message = "This will attempt to re-connect you to the student and fix sound issues. "
					  "The student will be disconnected and re-called. "
					  "Please do not hit this button twice. "
					  "This re-connect may take a few seconds. "
					  "Are you sure you want to continue?";
	if (QMessageBox::No == QMessageBox::warning(this, "TS", message, QMessageBox::Yes | QMessageBox::No))
		return;
	ApplicationController::server()->startInteractionReconnection(_interactionId);
	onLostConnection();
}

void FlashDialog::onLostConnection()
{
	reconnectingStatus(true);
}

void FlashDialog::onInteractionReconnected()
{
	reconnectingStatus(false);
}

void FlashDialog::reconnectingStatus(bool reconnecting)
{
	if (reconnecting)
	{
		_reconnection->setStyleSheet("background-color: #ffaaaa; color: #000;");
		_reconnection->setText("Reconnecting...");
		_reconnection->setEnabled(false);
		_timer->stop();
	}
	else
	{
		_reconnection->setStyleSheet("background-color: ; color: ;");
		_reconnection->setText("Reconnect");
		_reconnection->setEnabled(true);
		_timer->start();
	}
}

void FlashDialog::onInvokeMessage(QString msg)
{
	qDebug() << "Message: " << msg;
}

void FlashDialog::loadMovie(QString params)
{
	qDebug() << "Load Movie: " << params;
	_webView->page()->mainFrame()->evaluateJavaScript(params + _js);
}

void FlashDialog::onJSWindowObjectCleared()
{
	qDebug() << "Object Cleared";
	_webView->page()->mainFrame()->addToJavaScriptWindowObject("mainWindow", this);
}

void FlashDialog::onMuteClicked()
{
	if (_mute->isChecked()) {
		ApplicationController::fs()->unmute();
		_mute->setText("Mute");
		_mute->setChecked(false);
	}
	else {
		ApplicationController::fs()->mute();
		_mute->setText("Unmute");
		_mute->setChecked(true);
	}
}

void FlashDialog::copyInteractionId()
{
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setText(_interactionId, QClipboard::Clipboard);
}
