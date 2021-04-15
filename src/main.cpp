#include <config.h>
#include "Log4Esp.h"
#include "HardwareModules/HardwareRegistryFactory.h"

Logger _logger;
HardwareRegistry _hardwareRegistry;

void setup() {
  Serial.begin(9600);

  _logger = Logger("defaultLogger", false);
  _hardwareRegistry = HardwareRegistryFactory::createHardwareRegistry();
  _hardwareRegistry.reconnectAllDisconnectedDevices();
}

void loop()
{
    delay(1000);
}