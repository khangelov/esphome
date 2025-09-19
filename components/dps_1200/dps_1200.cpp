#include "dps_1200.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dps_1200 {

static const char *const TAG = "dps_1200";
void DPS1200::update() {
  uint8_t reg_addresses[6] = {0x08, 0x0a, 0x0e, 0x10, 0x1c, 0x1e};
  float stat;

    for (uint8_t i = 0; i < 6; i++) {
    uint8_t current_reg = reg_addresses[i];
    
    // --- Start of inlined read_dps1200_register logic ---
    // Calculate checksum
    uint8_t cs = (this->address_ << 1) + current_reg;
    uint8_t regCS = ((0xff - cs) + 1) & 0xff;

    // Write register address and checksum
    uint8_t write_buffer[2] = {current_reg, regCS};
    if (this->write(write_buffer, 2) != i2c::ERROR_OK) {
      ESP_LOGW(TAG, "I2C write failed for register 0x%02X. Skipping this register.", current_reg);
      continue; // Skip to the next register if write failed
    }
    esphome::delay(1); // Short delay as in original code

    // Read 3 bytes (LSB, MSB, Checksum/Status byte)
    uint8_t read_buffer[3];
    if (this->read(read_buffer, 3) != i2c::ERROR_OK) {
      ESP_LOGW(TAG, "I2C read failed for register 0x%02X. Skipping this register.", current_reg);
      continue; // Skip to the next register if read failed
    }

    // Original code: msg[0] = LSB, msg[1] = MSB
    uint16_t ret = (read_buffer[1] << 8) | read_buffer[0];
    // --- End of inlined read_dps1200_register logic ---

    // Process register data and publish state

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
}

/*
void DPS1200::update() {
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
}
*/

}  // namespace dps_1200
}  // namespace esphome
