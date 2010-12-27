#ifndef UTILS_H
#define UTILS_H

#include "trainer_studio.h"

namespace Utils
{
	void centerWindowOnDesktop(QWidget *window);
	void msgbox(QString msg);
	QVariantMap getSystemInfos();
	void trim_json(char *);
	QString formatCallerID(QString cidName, QString CidNumber);
}

#endif // UTILS_H
