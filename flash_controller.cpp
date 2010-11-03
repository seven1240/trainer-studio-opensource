#include "application_controller.h"
#include "flash_controller.h"
#include "freeswitch.h"

FlashController::FlashController(FlashDialog *view, QObject *parent) : Controller(parent)
{
	_view = view;
}

FlashController::~FlashController()
{
}
