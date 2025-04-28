#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace dps1200_sensor {

class DPS1200Sensor : public PollingComponent {
 public:
  sensor::Sensor *volt_in_sensor = nullptr;
  sensor::Sensor *amp_in_sensor = nullptr;
  sensor::Sensor *watt_in_sensor = nullptr;
  sensor::Sensor *volt_out_sensor = nullptr;
  sensor::Sensor *amp_out_sensor = nullptr;
  sensor::Sensor *watt_out_sensor = nullptr;
  sensor::Sensor *internal_temp_sensor = nullptr;
  sensor::Sensor *fan_rpm_sensor = nullptr;

  DPS1200Sensor() : PollingComponent(5000) {}

  void setup() override;
  void update() override;
  void dump_config() override;
};

}  // namespace dps1200_sensor
}  // namespace esphome
