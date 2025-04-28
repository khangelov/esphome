#include "dps1200_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dps1200_sensor {

static const char *const TAG = "dps1200_sensor";

void DPS1200Sensor::setup() {
  ESP_LOGCONFIG(TAG, "Setting up DPS1200 Sensor...");
}

void DPS1200Sensor::update() {
  // Simulated example (replace with real reads)
  float vin = 230.0;
  float ain = 5.0;
  float vout = 12.2;
  float aout = 100.0;
  float temp = 45.0;
  float rpm = 3200.0;

  float win = vin * ain;
  float wout = vout * aout;

  if (volt_in_sensor) volt_in_sensor->publish_state(vin);
  if (amp_in_sensor) amp_in_sensor->publish_state(ain);
  if (watt_in_sensor) watt_in_sensor->publish_state(win);
  if (volt_out_sensor) volt_out_sensor->publish_state(vout);
  if (amp_out_sensor) amp_out_sensor->publish_state(aout);
  if (watt_out_sensor) watt_out_sensor->publish_state(wout);
  if (internal_temp_sensor) internal_temp_sensor->publish_state(temp);
  if (fan_rpm_sensor) fan_rpm_sensor->publish_state(rpm);
}

void DPS1200Sensor::dump_config() {
  ESP_LOGCONFIG(TAG, "DPS1200 Sensor Config:");
  LOG_SENSOR("  ", "Input Voltage", this->volt_in_sensor);
  LOG_SENSOR("  ", "Input Current", this->amp_in_sensor);
  LOG_SENSOR("  ", "Input Power", this->watt_in_sensor);
  LOG_SENSOR("  ", "Output Voltage", this->volt_out_sensor);
  LOG_SENSOR("  ", "Output Current", this->amp_out_sensor);
  LOG_SENSOR("  ", "Output Power", this->watt_out_sensor);
  LOG_SENSOR("  ", "Internal Temperature", this->internal_temp_sensor);
  LOG_SENSOR("  ", "Fan RPM", this->fan_rpm_sensor);
}

}  // namespace dps1200_sensor
}  // namespace esphome
