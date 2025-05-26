#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace dps_1200 {

class HPPSUMonitor : public PollingComponent, public I2CDevice, public Sensor {
 public:
  
  sensor::Sensor *vin_sensor{nullptr};
  sensor::Sensor *vout_sensor{nullptr};
  sensor::Sensor *iin_sensor{nullptr};
  sensor::Sensor *iout_sensor{nullptr};
  sensor::Sensor *pin_sensor{nullptr};
  sensor::Sensor *pout_sensor{nullptr};
  sensor::Sensor *temp_sensor{nullptr};
  sensor::Sensor *fan_sensor{nullptr};

  HPPSUMonitor() : PollingComponent(5000), I2CDevice(0x58) {}

  void setup() override {}
  void update() override;

};

}  // namespace dps_1200
}  // namespace esphome
