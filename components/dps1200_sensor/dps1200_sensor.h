#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace dps1200_sensor {

class DPS1200Sensor : public PollingComponent {
 public:
  // Sensors
  sensor::Sensor *volt_in_sensor = nullptr;
  sensor::Sensor *amp_in_sensor = nullptr;
  sensor::Sensor *watt_in_sensor = nullptr;
  sensor::Sensor *volt_out_sensor = nullptr;
  sensor::Sensor *amp_out_sensor = nullptr;
  sensor::Sensor *watt_out_sensor = nullptr;
  sensor::Sensor *internal_temp_sensor = nullptr;
  sensor::Sensor *fan_rpm_sensor = nullptr;

  // Constructor with dynamic polling
  DPS1200Sensor(uint32_t update_interval = 5000) : PollingComponent(update_interval) {}

  void setup() override;
  void update() override;
  void dump_config() override;
};

}  // namespace dps1200_sensor
}  // namespace esphome
