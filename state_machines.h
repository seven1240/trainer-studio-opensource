#ifndef STATE_MACHINES_H
#define STATE_MACHINES_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QStateMachine;
QT_END_NAMESPACE

class StateMachineDebugger : public QObject
{
	Q_OBJECT

public:
	 StateMachineDebugger(QStateMachine *machine);

private:
	 QStateMachine *_machine;

private slots:
	void changed();
	
};

extern QStateMachine *jDebugStateMachine(QStateMachine *machine);

#endif // STATE_MACHINES_H
