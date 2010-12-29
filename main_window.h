#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QToolButton>
#include <QKeyEvent>
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
	void showEvent(QShowEvent *e);
	void changeEvent(QEvent *e);
	QLayout *createBody();

private:
	QPushButton *_state;
	QLabel *_sipStatusLabel;
	QToolButton *_sipReg;
	bool _sipStateReady;
	bool _isPaused;
	QString _activeUUID;
	QSystemTrayIcon *_systemTray;
	QTimer *_timer;
	void parseCallResult(QString res);

signals:
	void call();
	void testEcho();
	void testFlash();

private slots:
	void on_Hangup_clicked();
	void on_Call_clicked();
	void on_Echo_clicked();
	void on_State_clicked();
	void on_Flash_clicked();
	void on_About_clicked();
	void on_Logout_clicked();
	void on_Settings_clicked();
	void on_SipReg_clicked();
	void onPaused(bool);
	void onForcedPause(QString reason);
	void onGatewayStateChange(QString name, QString state);
	void onReservedForInteraction(QVariantMap);
	void onTimer();
};

#endif // MAIN_WINDOW_H
