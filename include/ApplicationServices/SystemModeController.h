#include "Ticker.h"

class SystemModeController
{
public:
    SystemModeController();
    void refresh();

private:
    void checkNightMode();

    Ticker* _nightModeTimer;
};