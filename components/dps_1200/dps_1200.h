#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace dps_1200 {

class HPPSUMonitor : public PollingComponent, public i2c::I2CDevice {
 public:
  sensor::Sensor *vin_sensor{nullptr};
  sensor::Sensor *vout_sensor{nullptr};
  sensor::Sensor *iin_sensor{nullptr};
  sensor::Sensor *iout_sensor{nullptr};
  sensor::Sensor *pin_sensor{nullptr};
  sensor::Sensor *pout_sensor{nullptr};
  sensor::Sensor *temp_sensor{nullptr};
  sensor::Sensor *fan_sensor{nullptr};

  void setup() override {}
  void update() override;

 protected:
  void read_and_publish(uint8_t reg, sensor::Sensor *sensor);
  float convert_linear11(uint8_t high, uint8_t low);
};

}  // namespace dps_1200
}  // namespace esphome
