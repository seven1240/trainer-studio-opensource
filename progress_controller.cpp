#include "application_controller.h"
#include "progress_controller.h"
#include "progress_widget.h"
#include "freeswitch.h"

ProgressController::ProgressController(ProgressWidget *view, QObject *parent) : Controller(parent)
{
	view->setModel(&_messagesModel);
	connect(ApplicationController::fs(), SIGNAL(loaded(QString, QString, QString)), this, SLOT(onModuleLoaded(QString, QString, QString)));
	connect(this, SIGNAL(changed()), view, SLOT(onHistoryChanged()));
}

ProgressController::~ProgressController()
{
}

void ProgressController::onModuleLoaded(QString modType, QString modKey, QString modName)
{
	_messages.append(QString("Loaded: [%1] %2 %3").arg(modType).arg(modKey).arg(modName));
	_messagesModel.setStringList(_messages);
	emit changed();
}
