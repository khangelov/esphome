#pragma once
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"

namespace esphome {
namespace dps1200 {

class DPS1200Sensor : public PollingComponent, public i2c::I2CDevice {
 public:
  DPS1200Sensor() = default;

  void set_sensors(sensor::Sensor *volt_in, sensor::Sensor *amp_in,
                   sensor::Sensor *volt_out, sensor::Sensor *amp_out,
                   sensor::Sensor *internal_temp, sensor::Sensor *fan_rpm) {
    volt_in_ = volt_in;
    amp_in_ = amp_in;
    volt_out_ = volt_out;
    amp_out_ = amp_out;
    internal_temp_ = internal_temp;
    fan_rpm_ = fan_rpm;
  }

  void update() override;

 protected:
  float f2c(float f) { return (f - 32.0f) * 5.0f / 9.0f; }

  sensor::Sensor *volt_in_{nullptr};
  sensor::Sensor *amp_in_{nullptr};
  sensor::Sensor *volt_out_{nullptr};
  sensor::Sensor *amp_out_{nullptr};
  sensor::Sensor *internal_temp_{nullptr};
  sensor::Sensor *fan_rpm_{nullptr};
};

}  // namespace dps1200
}  // namespace esphome
