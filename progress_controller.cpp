#include "application_controller.h"
#include "progress_controller.h"
#include "fs_host.h"

ProgressController::ProgressController(QObject *parent) : Controller(parent)
{
	connect(ApplicationController::fs(), SIGNAL(moduleLoaded(QString, QString, QString)), this, SLOT(onModuleLoaded(QString, QString, QString)));
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
