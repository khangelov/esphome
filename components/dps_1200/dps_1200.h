#pragma once
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"

namespace esphome {
namespace dps_1200 {

class DPS1200 : public PollingComponent, public i2c::I2CDevice {
 public:
  DPS1200() = default;

  void set_sensors(sensor::Sensor *volt_in, sensor::Sensor *amp_in,
                   sensor::Sensor *volt_out, sensor::Sensor *amp_out,
                   sensor::Sensor *internal_temp, sensor::Sensor *fan_rpm,
                   sensor::Sensor *watt_in, sensor::Sensor *watt_out) {
    volt_in_ = volt_in;
    amp_in_ = amp_in;
    volt_out_ = volt_out;
    amp_out_ = amp_out;
    internal_temp_ = internal_temp;
    fan_rpm_ = fan_rpm;
    watt_in_ = watt_in;
    watt_out_ = watt_out;
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
  sensor::Sensor *watt_in_{nullptr};
  sensor::Sensor *watt_out_{nullptr};

  float grid_v_{0}, grid_a_{0}, out_v_{0}, out_a_{0};
};

}  // namespace dps_1200
}  // namespace esphome