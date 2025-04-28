#include "dps1200_sensor.h"
#include "esphome.h"

namespace esphome {
namespace dps1200_sensor {

void DPS1200Sensor::setup() {
  Wire.begin();
}

void DPS1200Sensor::update() {
  uint8_t reg[6] = {0x08, 0x0a, 0x0e, 0x10, 0x1c, 0x1e};
  uint8_t i, cs, regCS;
  uint16_t msg[3];
  float stat;

  for (i = 0; i < 6; i++) {
    cs = (address_ << 1) + reg[i];
    regCS = ((0xff - cs) + 1) & 0xff;

    Wire.beginTransmission(address_);
    Wire.write(reg[i]);
    Wire.write(regCS);
    Wire.endTransmission();
    delay(1);

    Wire.requestFrom((int)address_, 3);
    msg[0] = Wire.read();
    msg[1] = Wire.read();
    msg[2] = Wire.read();
    uint16_t ret = (msg[1] << 8) + msg[0];

    switch (i) {
      case 0:
        stat = ret / 32.0;
        volt_in->publish_state(stat);
        break;
      case 1:
        stat = ret / 128.0;
        amp_in->publish_state(stat);
        break;
      case 2:
        stat = ret / 256.0;
        volt_out->publish_state(stat);
        break;
      case 3:
        stat = ret / 128.0;
        amp_out->publish_state(stat);
        break;
      case 4:
        stat = ret / 32.0;
        internal_temp->publish_state(f2c(stat));
        break;
      case 5:
        stat = ret;
        fan_rpm->publish_state(stat);
        break;
    }
  }
}

}  // namespace dps1200_sensor
}  // namespace esphome