#pragma once

#include "dps_1200.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"

namespace esphome {
namespace dps_1200 {

class HPPSUMonitorSensor : public HPPSUMonitor {
 public:
  void set_vin_sensor(sensor::Sensor *sensor) { this->vin_sensor = sensor; }
  void set_vout_sensor(sensor::Sensor *sensor) { this->vout_sensor = sensor; }
  void set_iin_sensor(sensor::Sensor *sensor) { this->iin_sensor = sensor; }
  void set_iout_sensor(sensor::Sensor *sensor) { this->iout_sensor = sensor; }
  void set_pin_sensor(sensor::Sensor *sensor) { this->pin_sensor = sensor; }
  void set_pout_sensor(sensor::Sensor *sensor) { this->pout_sensor = sensor; }
  void set_temp_sensor(sensor::Sensor *sensor) { this->temp_sensor = sensor; }
  void set_fan_sensor(sensor::Sensor *sensor) { this->fan_sensor = sensor; }
};

}  // namespace dps_1200
}  // namespace esphome
