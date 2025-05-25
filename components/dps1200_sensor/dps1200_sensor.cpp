#ifndef DPS1200_SENSOR_H
#define DPS1200_SENSOR_H

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace dps1200_sensor {

class DPS1200Sensor : public PollingComponent, public i2c::I2CDevice {
 public:
  void setup() override;
  void update() override;

  // Sensor setters
  void set_vin_sensor(sensor::Sensor *sensor) { vin_sensor_ = sensor; }
  void set_iin_sensor(sensor::Sensor *sensor) { iin_sensor_ = sensor; }
  void set_pin_sensor(sensor::Sensor *sensor) { pin_sensor_ = sensor; }
  void set_vout_sensor(sensor::Sensor *sensor) { vout_sensor_ = sensor; }
  void set_iout_sensor(sensor::Sensor *sensor) { iout_sensor_ = sensor; }
  void set_pout_sensor(sensor::Sensor *sensor) { pout_sensor_ = sensor; }

 protected:
  sensor::Sensor *vin_sensor_{nullptr};
  sensor::Sensor *iin_sensor_{nullptr};
  sensor::Sensor *pin_sensor_{nullptr};
  sensor::Sensor *vout_sensor_{nullptr};
  sensor::Sensor *iout_sensor_{nullptr};
  sensor::Sensor *pout_sensor_{nullptr};

  float decode_linear11(uint16_t val);
  float read_pmbus(uint8_t command);
};

}  // namespace dps1200_sensor
}  // namespace esphome

#endif  // DPS1200_SENSOR_H