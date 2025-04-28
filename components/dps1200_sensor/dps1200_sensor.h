#pragma once
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace dps1200_sensor {

class DPS1200Sensor : public PollingComponent, public i2c::I2CDevice {
 public:
  void setup() override;
  void loop() override;
  void update() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_volt_in_sensor(sensor::Sensor *sensor) { volt_in_ = sensor; }
  void set_amp_in_sensor(sensor::Sensor *sensor) { amp_in_ = sensor; }
  void set_watt_in_sensor(sensor::Sensor *sensor) { watt_in_ = sensor; }
  void set_volt_out_sensor(sensor::Sensor *sensor) { volt_out_ = sensor; }
  void set_amp_out_sensor(sensor::Sensor *sensor) { amp_out_ = sensor; }
  void set_watt_out_sensor(sensor::Sensor *sensor) { watt_out_ = sensor; }
  void set_internal_temp_sensor(sensor::Sensor *sensor) { internal_temp_ = sensor; }
  void set_fan_rpm_sensor(sensor::Sensor *sensor) { fan_rpm_ = sensor; }

  void set_address(uint8_t address) { this->address_ = address; }  // Configurable I2C address

 protected:
  sensor::Sensor *volt_in_{nullptr};
  sensor::Sensor *amp_in_{nullptr};
  sensor::Sensor *watt_in_{nullptr};
  sensor::Sensor *volt_out_{nullptr};
  sensor::Sensor *amp_out_{nullptr};
  sensor::Sensor *watt_out_{nullptr};
  sensor::Sensor *internal_temp_{nullptr};
  sensor::Sensor *fan_rpm_{nullptr};
  uint8_t address_;
};

}  // namespace dps1200_sensor
}  // namespace esphome