/*
 * Eleutian Trainer Studio Application Opensource Edition, a VoIP client with Webkit and Flash support.
 * Copyright (C) 2011, Eleutian Technology, LLC <http://www.eleutian.com>
 *
 * Version: Apache License 2.0
 *
 * The content of this file is licensed under the Apache License, Version 2.0. (the "License").
 * You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.apache.org/licenses/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Eleutian Trainer Studio Application Opensource Edition.
 *
 * The Initial Developer of the Original Code is
 * Seven Du <seven@eleutian.com>
 * Portions created by the Initial Developer are Copyright (C)
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Jadob Lewallen <jacob@eleutian.com>
 */
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
class QAction;
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

	QAction *_test;
	QAction *_mute;
	QAction *_reconnect;
	QLabel *_reconnectLabel;
	QLabel *_time;
	QWebView *_webView;
	QString _js;
	QString _interactionId;
	QVariantMap _interactionData;

	int _currentMovie;
	int _seconds;
	int _interactionSeconds;
	QTimer *_timer;

	void loadMovie(QString params);
	void loadInteractionMovie();
	void loadReviewMovie();
	void reconnectingStatus(bool reconnecting);
};

#endif // FLASHDIALOG_H
