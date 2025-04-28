#include <dps1200_sensor.h>
#include <esphome/core/log.h>

namespace esphome {
namespace dps1200_sensor {

static const char *const TAG = "dps1200_sensor";

float f2c(uint16_t temp) {
  return (temp - 32) * 0.5556;
}

void DPS1200Sensor::setup() {
  ESP_LOGCONFIG(TAG, "Setting up DPS1200 Sensor at address 0x%02X...", this->address_);
}

void DPS1200Sensor::loop() {}

void DPS1200Sensor::update() {
  uint8_t reg[6] = {0x08, 0x0a, 0x0e, 0x10, 0x1c, 0x1e};
  uint8_t cs, regCS;
  uint16_t msg[3];
  float stat;

  for (uint8_t i = 0; i < 6; i++) {
    cs = (this->address_ << 1) + reg[i];
    regCS = ((0xff - cs) + 1) & 0xff;

    if (!this->write_bytes(reg[i], &regCS, 1)) {
      ESP_LOGW(TAG, "Failed to write to register 0x%02X", reg[i]);
      continue;
    }

    delay(1);

    uint8_t data[3];
    if (!this->read_bytes_raw(data, 3)) {
      ESP_LOGW(TAG, "Failed to read from register 0x%02X", reg[i]);
      continue;
    }
    msg[0] = data[0];
    msg[1] = data[1];
    msg[2] = data[2];

    uint16_t ret = (msg[1] << 8) + msg[0];

    if (i == 0) {
      stat = ret / 32.0;
      ESP_LOGD(TAG, "Grid Voltage: %.2f V", stat);
      if (volt_in_) volt_in_->publish_state(stat);
    } else if (i == 1) {
      stat = ret / 128.0;
      ESP_LOGD(TAG, "Grid Current: %.3f A", stat);
      if (amp_in_) amp_in_->publish_state(stat);
    } else if (i == 2) {
      stat = ret / 256.0;
      ESP_LOGD(TAG, "Output Voltage: %.2f V", stat);
      if (volt_out_) volt_out_->publish_state(stat);
    } else if (i == 3) {
      stat = ret / 128.0;
      ESP_LOGD(TAG, "Output Current: %.3f A", stat);
      if (amp_out_) amp_out_->publish_state(stat);
    } else if (i == 4) {
      stat = ret / 32.0;
      float temp_c = f2c(stat);
      ESP_LOGD(TAG, "Internal Temp: %.1f °C", temp_c);
      if (internal_temp_) internal_temp_->publish_state(temp_c);
    } else if (i == 5) {
      stat = ret;
      ESP_LOGD(TAG, "Fan RPM: %.0f RPM", stat);
      if (fan_rpm_) fan_rpm_->publish_state(stat);
    }
  }

  if (volt_in_ && amp_in_ && watt_in_) {
    float power = (volt_in_->get_state() * amp_in_->get_state());
    ESP_LOGD(TAG, "Input Power: %.1f W", power);
    watt_in_->publish_state(power);
  }
  if (volt_out_ && amp_out_ && watt_out_) {
    float power = (volt_out_->get_state() * amp_out_->get_state());
    ESP_LOGD(TAG, "Output Power: %.1f W", power);
    watt_out_->publish_state(power);
  }
}

}  // namespace dps1200_sensor
}  // namespace esphome