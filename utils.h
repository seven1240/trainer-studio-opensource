#ifndef UTILS_H
#define UTILS_H

#include "trainer_studio.h"

namespace Utils
{
  void centerWindowOnDesktop(QWidget *window);
  void msgbox(QString msg);
  QVariantMap getSystemInfos();
}

#endif // UTILS_H
