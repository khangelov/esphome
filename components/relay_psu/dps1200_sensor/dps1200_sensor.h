#pragma once
#include "esphome.h"

namespace esphome {
namespace dps1200_sensor {

class DPS1200Sensor : public PollingComponent {
 public:
  DPS1200Sensor() : PollingComponent(15000) {}

  Sensor *volt_in{new Sensor()};
  Sensor *amp_in{new Sensor()};
  Sensor *watt_in{new Sensor()};
  Sensor *volt_out{new Sensor()};
  Sensor *amp_out{new Sensor()};
  Sensor *watt_out{new Sensor()};
  Sensor *internal_temp{new Sensor()};
  Sensor *fan_rpm{new Sensor()};

  void set_address(uint8_t address) { address_ = address; }

  void setup() override;
  void update() override;

 protected:
  uint8_t address_ = 0x58;
  float f2c(uint16_t temp) { return (temp - 32) * 0.5556; }
};

}  // namespace dps1200_sensor
}  // namespace esphome