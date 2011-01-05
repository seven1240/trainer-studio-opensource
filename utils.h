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
	/* compare version in <major.minor.rev>, returns
		3 - major2 > major1
		2 - minor2 > minor1
		1 - rev2 > rev1
		0 - equel
		-1 - rev2 < rev1
		-2 - error
	*/
	int compareVersion(QString ver1, QString ver2);
	void openFolder(QString path);
	QString getCacheDir();
	void clearWebCache();
}

#endif // UTILS_H
