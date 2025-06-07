#include "dps_1200.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dps_1200 {

static const char *const TAG = "dps_1200";



void DPS1200::update() {
  uint8_t reg_addresses[6] = {0x08, 0x0a, 0x0e, 0x10, 0x1c, 0x1e};
  float stat;
  optional<uint16_t> raw_value;

  for (uint8_t i = 0; i < 6; i++) {
    raw_value = this->read_dps1200_register(reg_addresses[i]);

    if (!raw_value.has_value()) {
      ESP_LOGW(TAG, "Failed to read from DPS1200 register 0x%02X. Skipping this register.", reg_addresses[i]);
      continue; // Skip to the next register if read failed
    }

    uint16_t ret = raw_value.value();

    // Process register data and publish state
    switch (i) {
      case 0: // Grid Voltage (0x08)
        if (this->volt_in_ != nullptr) {
          stat = ret / 32.0;
          this->volt_in_->publish_state(stat);
        }
        break;
      case 1: // Grid Current (0x0A)
        if (this->amp_in_ != nullptr) {
          stat = ret / 128.0;
          this->amp_in_->publish_state(stat);
        }
        break;
      case 2: // Output Voltage (0x0E)
        if (this->volt_out_ != nullptr) {
          stat = ret / 256.0;
          this->volt_out_->publish_state(stat);
        }
        break;
      case 3: // Output Current (0x10)
        if (this->amp_out_ != nullptr) {
          stat = ret / 128.0;
          this->amp_out_->publish_state(stat);
        }
        break;
      case 4: // Internal Temperature (0x1C)
        if (this->internal_temp_ != nullptr) {
          stat = ret / 32.0;
          this->internal_temp_->publish_state(esphome::f2c(stat)); // Using ESPHome's f2c
        }
        break;
      case 5: // Fan RPM (0x1E)
        if (this->fan_rpm_ != nullptr) {
          stat = ret; // No division needed for RPM
          this->fan_rpm_->publish_state(stat);
        }
        break;
    }

 /*
  const uint8_t reg[6] = {0x08, 0x0a, 0x0e, 0x10, 0x1c, 0x1e};
  uint16_t msg[3];

  for (uint8_t i = 0; i < 6; i++) {
    uint8_t cs = (this->address_ << 1) + reg[i];
    uint8_t regCS = ((0xff - cs) + 1) & 0xff;

    uint8_t buffer[2] = {reg[i], regCS};
    this->write(buffer, 2);
    delay(1);
    this->read((uint8_t *)msg, 3);
    uint16_t ret = (msg[1] << 8) | msg[0];
    float stat = 0.0f;

    switch (i) {
      case 0:
        stat = ret / 32.0f;
        grid_v_ = stat;
        if (volt_in_) volt_in_->publish_state(stat);
        break;
      case 1:
        stat = ret / 128.0f;
        grid_a_ = stat;
        if (amp_in_) amp_in_->publish_state(stat);
        break;
      case 2:
        stat = ret / 256.0f;
        out_v_ = stat;
        if (volt_out_) volt_out_->publish_state(stat);
        break;
      case 3:
        stat = ret / 128.0f;
        out_a_ = stat;
        if (amp_out_) amp_out_->publish_state(stat);
        break;
      case 4:
        stat = ret / 32.0f;
        if (internal_temp_) internal_temp_->publish_state(f2c(stat));
        break;
      case 5:
        stat = ret;
        if (fan_rpm_) fan_rpm_->publish_state(stat);
        break;
    }
        
  }

  if (watt_in_ && grid_v_ > 0 && grid_a_ > 0)
    watt_in_->publish_state(grid_v_ * grid_a_);
  if (watt_out_ && out_v_ > 0 && out_a_ > 0)
    watt_out_->publish_state(out_v_ * out_a_);
    */
}

}  // namespace dps_1200
}  // namespace esphome