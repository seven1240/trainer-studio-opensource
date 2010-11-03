#ifndef PROGRESS_CONTROLLER_H
#define PROGRESS_CONTROLLER_H

#include "controller.h"

class ProgressController : public Controller
{
	Q_OBJECT
public:
	ProgressController(QObject *parent = 0);
	~ProgressController();

private slots:
	void onModuleLoaded(QString modType, QString modKey, QString modName);

private:
	QString _status;
	QList<QString> _messages;

};

#endif // PROGRESS_CONTROLLER_H
