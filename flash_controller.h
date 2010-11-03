#ifndef FLASH_CONTROLLER_H
#define FLASH_CONTROLLER_H

#include <QStringListModel>

#include "controller.h"

class FlashDialog;

class FlashController : public Controller
{
	Q_OBJECT
public:
	FlashController(FlashDialog *view, QObject *parent = 0);
	~FlashController();

public:

private:
	FlashDialog *_view;
};

#endif // FLASH_CONTROLLER_H
