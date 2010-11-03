#ifndef PROGRESS_CONTROLLER_H
#define PROGRESS_CONTROLLER_H

#include <QStringListModel>

#include "controller.h"

class ProgressController : public Controller
{
	Q_OBJECT
public:
	ProgressController(QObject *parent = 0);
	~ProgressController();

public:
	QStringListModel &messageModel() {
		return _messagesModel;
	}

signals:
	void changed();

private slots:
	void onModuleLoaded(QString modType, QString modKey, QString modName);

private:
	QString _status;
	QList<QString> _messages;
	QStringListModel _messagesModel;

};

#endif // PROGRESS_CONTROLLER_H
