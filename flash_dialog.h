#ifndef FLASHDIALOG_H
#define FLASHDIALOG_H

#include <QDialog>
#include <QVariant.h>
#include <QDateTime.h>

QT_BEGIN_NAMESPACE
class QWebView;
class QPushButton;
class QLabel;
class QTimer;
QT_END_NAMESPACE

class FlashDialog : public QDialog {
	Q_OBJECT

public:
	FlashDialog(QWidget *parent = 0);
	~FlashDialog();

protected:
	void changeEvent(QEvent *e);
	void showEvent(QShowEvent *e);
	void closeEvent(QCloseEvent *e);

public slots:
	void onFSCommand(QString cmd, QString args);
	void onJSWindowObjectCleared();

private slots:
	void onMuteClicked();
	void onReconnectClicked();
	void onTestClicked();
	void onDisconnectClicked();
	void onReservedForInteraction(QVariantMap);
	void onLoadFinished(bool);
	void onLostConnection();
	void onInteractionReconnected();
	void onInvokeMessage(QString);
	void onTimer();

signals:
	void closed();

private:
	QPushButton *_disconnect;
	QPushButton *_reconnect;
	QPushButton *_mute;
	QPushButton *_test;
	QLabel *_time;
	QWebView *_webView;
	QString _js;
	QString _interactionId;
	void loadMovie(QString params);
	int _seconds;
	QTimer *_timer;
};

#endif // FLASHDIALOG_H
