#include "dps_1200.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dps_1200 {

static const char *const TAG = "hp_psu";

void HPPSUMonitor::update() {
  read_and_publish(0x88, vin_sensor);
  read_and_publish(0x8B, vout_sensor);
  read_and_publish(0x89, iin_sensor);
  read_and_publish(0x8C, iout_sensor);
  read_and_publish(0x96, pin_sensor);
  read_and_publish(0x98, pout_sensor);
  read_and_publish(0x97, temp_sensor);
  read_and_publish(0x90, fan_sensor);
}

void HPPSUMonitor::read_and_publish(uint8_t reg, sensor::Sensor *sensor) {
  uint8_t buffer[2];
  if (this->write(&reg, 1) && this->read(buffer, 2)) {
    float val = convert_linear11(buffer[1], buffer[0]);
    sensor->publish_state(val);
  } else {
    ESP_LOGW(TAG, "Failed to read register 0x%02X", reg);
  }
}

float HPPSUMonitor::f2c(uint16_t temp) {
  return (temp- 32) *.5556;
}

}  // namespace dps_1200
}  // namespace esphome
