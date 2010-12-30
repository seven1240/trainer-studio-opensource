#ifndef FLASHDIALOG_H
#define FLASHDIALOG_H

#include <QDialog>
#include <QVariant.h>
#include <QDateTime.h>

#define MOVIE_INTERACTION 1
#define MOVIE_REVIEW 2

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
	void onReconnectionClicked();
	void onTestClicked();
	void onReloadMovieClicked();
	void onHangupClicked();
	void onCallHangup(QString uuid, QString cidNumber, QString cidName);
	void onReservedForInteraction(QVariantMap);
	void onLoadFinished(bool);
	void onLostConnection();
	void onInteractionReconnected();
	void onInvokeMessage(QString);
	void onTimer();
	void copyInteractionId();

signals:
	void closed();

private:
	QPushButton *_hangup;
	QPushButton *_reconnection;
	QPushButton *_mute;
	QPushButton *_test;
	QPushButton *_reloadMovie;
	QLabel *_time;
	QWebView *_webView;
	QString _js;
	QString _interactionId;
	QVariantMap _interactionData;
	int _currentMovie;
	void loadMovie(QString params);
	void loadInteractionMovie();
	void loadReviewMovie();
	void reconnectingStatus(bool reconnecting);

	int _seconds;
	QTimer *_timer;
};

#endif // FLASHDIALOG_H
