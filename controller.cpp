#include <QStateMachine>
#include <QDebug>
#include "controller.h"
#include "state_machines.h"

Controller::Controller(QObject *parent) :
	QObject(parent)
{
	_state_machine = NULL;
}

Controller::~Controller()
{
}

QStateMachine *Controller::stateMachine()
{
	if (_state_machine == NULL) {
		_state_machine = jDebugStateMachine(createStateMachine());
	}
	return _state_machine;
}

void Controller::startStateMachine()
{
	stateMachine()->start();
}

QStateMachine *Controller::createStateMachine()
{
	return NULL;
}
