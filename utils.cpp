#include <QMessageBox.h>
#include <QtGui/QApplication>
#include <QtGlobal>
#include <QSysInfo>
#include <QProcess>
#include <QDesktopWidget>
#include "utils.h"

namespace Utils {
  void msgbox(QString msg)
  {
    QMessageBox::information(NULL, QApplication::applicationName(), msg);
  }

  void centerWindowOnDesktop(QWidget *window)
  {
    QDesktopWidget *desktop = QApplication::desktop();
    QWidget *screen = desktop->screen(desktop->primaryScreen());
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
}
