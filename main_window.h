#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QKeyEvent>
#include "flash_dialog.h"
#include "login_dialog.h"
#include "incoming_call_dialog.h"
#include "settings_dialog.h"
#include "trainer_studio.h"
#include "fs_host.h"
#include "user.h"

class MainWindow : public QWidget {
	Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
	void showLoginDialog();
	User *getUser() {
		return _user;
	}

protected:
	void changeEvent(QEvent *e);
	QLayout *createBody();

private:
	MainWindow *ui;
	QPushButton *btnState;
	QLabel *lbStatus;
	QLabel *lbSIPStatus;
	FlashDialog *flash_dialog;
	LoginDialog *login_dialog;
	IncomingCallDialog *incoming_call_dialog;
	SettingsDialog *settings_dialog;
	bool _sipStateReady;
	QString _activeUUID;
	void keyPressEvent(QKeyEvent *event);
	void parseCallResult(QString res);
	QTimer *_timer;
	User *_user;

private slots:
	void on_Hangup_clicked();
	void on_Conference_clicked();
	void on_Echo_clicked();
	void on_Settings_clicked();
	void on_State_clicked();
	void on_Flash_clicked();
	void on_About_clicked();
	void on_Login_clicked();
	void onAuthenticated(User *user);
	void onPaused(bool);
	void onForcedPause(QString reason);
	void onAnswered(QString cid_name, QString cid_number);
	void onGatewayStateChange(QString state);
	void onReservedForInteraction(QVariantMap);
	void onSocketDisconnected();
	void onNewEvent(QSharedPointer<switch_event_t>);
	void onTimerTimeout();
};

#endif // MAINWINDOW_H
