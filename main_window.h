#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QKeyEvent>
#include "flash_dialog.h"
#include "login_dialog.h"
#include "incoming_call_dialog.h"
#include "settings_dialog.h"
#include "trainer_studio.h"
#include "freeswitch.h"
#include "user.h"

QT_BEGIN_NAMESPACE
class QSystemTrayIcon;
QT_END_NAMESPACE

class MainWindow : public QWidget {
	Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	void changeEvent(QEvent *e);
	QLayout *createBody();

private:
	QPushButton *_state;
	QLabel *_sipStatusLabel;
	bool _sipStateReady;
	QString _activeUUID;
	QSystemTrayIcon *_systemTray;
	QTimer *_timer;
	void keyPressEvent(QKeyEvent *event);
	void parseCallResult(QString res);

signals:
	void beginEcho();
	void beginFlashDemo();

private slots:
	void on_Hangup_clicked();
	void on_Conference_clicked();
	void on_Echo_clicked();
	void on_State_clicked();
	void on_Flash_clicked();
	void on_About_clicked();
	void on_Logout_clicked();
	void on_Close_clicked();
	void onPaused(bool);
	void onForcedPause(QString reason);
	void onGatewayStateChange(QString name, QString state);
	void onReservedForInteraction(QVariantMap);
	void onTimer();
};

#endif // MAIN_WINDOW_H
