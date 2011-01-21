#include <QMessageBox.h>
#include <QtGui/QApplication>
#include <QtGlobal>
#include <QSysInfo>
#include <QProcess>
#include <QDesktopWidget>
#include <QDir>
#include "utils.h"

namespace Utils {
	void msgbox(QString msg)
	{
		QMessageBox::information(NULL, QApplication::applicationName(), msg);
	}

	void centerWindowOnDesktop(QWidget *window)
	{
		QDesktopWidget *desktop = QApplication::desktop();
		QRect screenGeometry = desktop->screenGeometry(desktop->primaryScreen());
		window->move(screenGeometry.width() / 2 - window->width() / 2,
					 screenGeometry.height() / 2 - window->height() / 2);
	}

	QVariantMap getSystemInfos()
	{
		QVariantMap map;
		QString os;
		QString memory = "0";
		QString screen_res = "Unknown";
		QString flash_player_version = "Unknown";

#ifdef Q_WS_WIN
		switch (QSysInfo::WindowsVersion) {
		case QSysInfo::WV_NT:        os = "Windows NT";      break;
		case QSysInfo::WV_2000:      os = "Windows 2000";    break;
		case QSysInfo::WV_XP:        os = "Windows XP";      break;
		case QSysInfo::WV_2003:      os = "Windows 2003";    break;
		case QSysInfo::WV_VISTA:     os = "Windows Vista";   break;
		case QSysInfo::WV_WINDOWS7:  os = "Windows 7";       break;
		default:                     os = "Windows Unknown";
		}
#endif
#ifdef Q_WS_MAC
		switch (QSysInfo::MacintoshVersion) {
		case QSysInfo::MV_LEOPARD:     os = "Mac Leopard";          break;
		case QSysInfo::MV_SNOWLEOPARD: os = "Mac Snow Leopard";     break;
		default:                       os = "Mac Unknown(unsupported)";
		}

		QProcess p;
		p.start("sysctl hw.memsize");
		if (p.waitForFinished()) {
			QByteArray ba = p.readAll();
			if (ba.startsWith("hw.memsize: ")) {
				ba = ba.trimmed().right(ba.length() - 13);
				memory = QString(ba);
			}
		}

#endif
		screen_res = QString("%1x%2").arg(QApplication::desktop()->width()).arg(QApplication::desktop()->height());
		map.insert("os", os);
		map.insert("memory", memory);
		map.insert("screen_res", screen_res);
		map.insert("flash_player_version", flash_player_version);
		return map;
	}

	/* trim \n \t off from json, will change the original string */
	void trim_json(char *s) {
		char *p = s;

		while(*s) {
			if (*s == '\n' || *s == '\t') {
				s++;
			} else {
				*p++ = *s++;
			}
		}
		if (p != s) {
			*p = '\0';
		}
	}

	QString formatCallerID(QString cidName, QString cidNumber)
	{
		return QString("\"%1\" <%2>").arg(cidName).arg(cidNumber);
	}

	int compareVersion(QString ver1, QString ver2)
	{
		QStringList list1 = ver1.split(".");
		QStringList list2 = ver2.split(".");

		int major1, minor1, rev1;
		int major2, minor2, rev2;

		if (list1.count() < 3) return -2;
		if (list2.count() < 3) return -2;

		major1 = list1[0].toInt();
		minor1 = list1[1].toInt();
		rev1 = list1[2].toInt();
		major2 = list2[0].toInt();
		minor2 = list2[1].toInt();
		rev2 = list2[2].toInt();

		if (major2 > major1) {
			return 3;
		} else if (major2 < major1) {
			return -1;
		} else {
			if (minor2 > minor1) {
				return 2;
			} else if (minor2 < minor1) {
				return -1;
			} else {
				if (rev2 > rev1) {
					return 1;
				} else if (rev2 < rev1) {
					return -1;
				} else {
					return 0;
				}
			}
		}
	}
	
	void openFolder(QString path)
	{
		#ifdef Q_WS_MAC
		    QStringList args;
		    args << "-e";
		    args << "tell application \"Finder\"";
		    args << "-e";
		    args << "activate";
		    args << "-e";
		    args << "tell application \"Finder\" to reveal POSIX file \""+path+"\"";
		    args << "-e";
		    args << "end tell";
		    QProcess::startDetached("osascript", args);
		#endif

		#ifdef Q_WS_WIN
		    QStringList args;
		    args << "/select," << QDir::toNativeSeparators(path);
		    QProcess::startDetached("explorer", args);
		#endif
	}
	
	QString getCacheDir()
	{
		QDir home = QDir::home();
		QString path = home.absoluteFilePath(DOTDIR "/cache");
		qDebug() << path;
		return path;
	}
	void clearWebCache()
	{
		QString cacheDir = QDir::toNativeSeparators(getCacheDir());

		QStringList cmdList;
		#ifdef Q_WS_WIN
			cmdList << "del /y \"" + cacheDir + "\\http\\cache*.cache\"";
			cmdList << "del /y \"" + cacheDir + "\\prepared\\cache*.cache\"";
		#else
			cmdList << "rm -f " + cacheDir + "/http/cache*.cache";
			cmdList << "rm -f " + cacheDir + "/prepared/cache*.cache";
		#endif
		
		for (int i=0; i<cmdList.length(); i++) {
			qDebug() << "Clearing cache: " << cmdList[i];
			system(cmdList[i].toAscii());
		}
	}
}
