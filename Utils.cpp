#include "Utils.h"
#include <qmessagebox.h>
#include <QtGui/QApplication>
#include <QtGlobal>
#include <QSysInfo>

namespace Utils {
    void msgbox(QString msg)
    {
        QMessageBox::information(NULL, QApplication::applicationName(), msg);
    }

    QVariantMap getSystemInfos()
    {
        QVariantMap map;
        QString os;

#ifdef Q_WS_WIN
        switch (QSysInfo::WindowsVersion) {
        case QSysInfo::WV_NT:        os = "Windows NT";      break;
        case QSysInfo::WV_2000:      os = "Windows 2000";    break;
        case QSysInfo::WV_XP:        os = "Windows XP";      break;
        case QSysInfo::WV_2003:      os = "Windows 2003";    break;
        case QSysInfo::WV_VISTA:     os = "Windows Vista";   break;
        case QSysInfo::WV_WINDOWS7   os = "Windows 7";       break;
        case QSysInfo::WV_2008:      os = "Windows 2008";    break;
        default:                     os = "Windows Unknown";
        }
#endif
#ifdef Q_WS_MAC
        switch (QSysInfo::MacintoshVersion) {
        case QSysInfo::MV_LEOPARD:   os = "Mac Leopard";            break;
        case QSysInfo::MV_SNOWLEOPARD: os = "Mac Snow Leopard";     break;
        default:                     os = "Mac Unknown(unsupported)";
        }
#endif
        map.insert("os", os);
        return map;
    }

}
