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

  void update() override;
  void setup() override;
  void loop() override;
  void dump_config() override;

};

}  // namespace dps_1200
}  // namespace esphome
