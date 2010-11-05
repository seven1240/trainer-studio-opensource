#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QStateMachine;
QT_END_NAMESPACE

class Controller : public QObject
{
	Q_OBJECT

public:
	Controller(QObject *parent = 0);
	~Controller();

protected:
	virtual QStateMachine *createStateMachine();
	QStateMachine *stateMachine();
	void startStateMachine();

private:
	QStateMachine *_state_machine;
};

#endif // CONTROLLER_H
