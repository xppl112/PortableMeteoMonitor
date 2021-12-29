#include "HardwareModules/HardwareRegistryFactory.h"
#include "config.h"

HardwareRegistry* HardwareRegistryFactory::createHardwareRegistry(){
  _hardwareRegistry = new HardwareRegistry();

  _hardwareRegistry->registerDevice<AirParticiplesSensor>(
    (HardwareId)HardwareIdList::AIR_PARTICIPLES_SENSOR, 
    new AirParticiplesSensor(PMS_RX_PIN, PMS_TX_PIN));

  _hardwareRegistry->registerDevice<CH2OSensor>(
    (HardwareId)HardwareIdList::CH2O_SENSOR, 
    new CH2OSensor(ZE08CH2O_RX_PIN, ZE08CH2O_TX_PIN));

  _hardwareRegistry->registerDevice<CO2Sensor>(
    (HardwareId)HardwareIdList::CO2_SENSOR, 
    new CO2Sensor());

  _hardwareRegistry->registerDevice<MeteoSensor>(
    (HardwareId)HardwareIdList::METEO_SENSOR, 
    new MeteoSensor(BME_I2C_ADDR));

  _hardwareRegistry->registerDevice<LightSensor>(
    (HardwareId)HardwareIdList::LIGHT_SENSOR, 
    new LightSensor(LIGHT_SENSOR_PIN));

  auto mcpExtender = new MCPExtender(MCP_EXTENDER_I2C_ADDR);
  _hardwareRegistry->registerDevice<MCPExtender>(
    (HardwareId)HardwareIdList::MCP_EXTENDER, mcpExtender);

  auto tftScreen = new TFTScreen(TFT_WIDTH, TFT_HEIGHT, TFT_CS, TFT_DC, 
      TFT_RST, TFT_BACKLIGHT, TFT_SPI_MODE, true, true);
  tftScreen->useMCP(mcpExtender->get());
  _hardwareRegistry->registerDevice<TFTScreen>(
    (HardwareId)HardwareIdList::TFT_SCREEN, tftScreen);

  _hardwareRegistry->registerDevice<BuzzerModule>(
    (HardwareId)HardwareIdList::BUZZER, 
    new BuzzerModule(mcpExtender->get(), BUZZER_PIN));

   return _hardwareRegistry;
}
