#include <QDebug>
#include <QStateMachine>
#include <QObjectList>
#include "state_machines.h"

StateMachineDebugger::StateMachineDebugger(QStateMachine *machine)
	: QObject(machine)
{
	_machine = machine;
	QList<QAbstractState*> children = _machine->findChildren<QAbstractState *>();
	QList<QAbstractState*>::iterator i;
	for (i = children.begin(); i != children.end(); ++i)
		connect(*i, SIGNAL(entered()), this, SLOT(changed()));
}

void StateMachineDebugger::changed()
{
	QSetIterator<QAbstractState*> i(_machine->configuration());
	while (i.hasNext())
	{
		qDebug() << QString("%1 -> %2").arg(_machine->objectName()).arg(i.next()->objectName());
	}
}

QStateMachine *jDebugStateMachine(QStateMachine *machine)
{
	new StateMachineDebugger(machine);
	return machine;
}
