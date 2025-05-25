#include "esphome/core/log.h"
#include "dps1200_sensorh"

namespace esphome {
namespace empty_compound_sensor {

static const char *TAG = "dps1200_sensor.sensor";

void dps1200Sensor::setup() {
  
}

void dps1200Sensor::loop() {
  
}

void Edps1200_ensor::update() {
    if (this->sensor1_ != nullptr)
      this->sensor1_->publish_state(1.0f);
    if (this->sensor2_ != nullptr)
      this->sensor2_->publish_state(2.0f);
    if (this->sensor3_ != nullptr)
      this->sensor3_->publish_state(3.0f);
}

void dps1200Sensor::dump_config() {
    ESP_LOGCONFIG(TAG, "Empty dps1200 sensor");
}

} //namespace empty_compound_sensor
} //namespace esphome