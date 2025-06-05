#include "dps1200_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dps1200 {

static const char *const TAG = "dps1200";

void DPS1200Sensor::update() {
  const uint8_t reg[6] = {0x08, 0x0a, 0x0e, 0x10, 0x1c, 0x1e};
  uint16_t msg[3];

  for (uint8_t i = 0; i < 6; i++) {
    uint8_t cs = (this->address_ << 1) + reg[i];
    uint8_t regCS = ((0xff - cs) + 1) & 0xff;

    this->write((uint8_t[]){reg[i], regCS}, 2);
    delay(1);  // Short delay

    this->read((uint8_t *)msg, 3);
    uint16_t ret = (msg[1] << 8) | msg[0];
    float stat = 0.0f;

    switch (i) {
      case 0:
        stat = ret / 32.0f;
        if (volt_in_) volt_in_->publish_state(stat);
        break;
      case 1:
        stat = ret / 128.0f;
        if (amp_in_) amp_in_->publish_state(stat);
        break;
      case 2:
        stat = ret / 256.0f;
        if (volt_out_) volt_out_->publish_state(stat);
        break;
      case 3:
        stat = ret / 128.0f;
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
}

}  // namespace dps1200
}  // namespace esphome
