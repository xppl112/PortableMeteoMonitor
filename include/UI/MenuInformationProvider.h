#include "Healthchecks/Healthchecks.h"

class MenuInformationProvider
{
public:
    MenuInformationProvider(Healthchecks* healthchecks);
    void getDevicesInfo();

private:
    Healthchecks* _healthchecks;
};