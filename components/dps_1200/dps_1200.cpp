#include "esphome/core/log.h"

namespace esphome {
namespace dps_1200 {

static const char *const TAG = "dps_1200";

void HPPSUMonitor::update() {
  const uint8_t reg[6] = {0x88, 0x89, 0x8B, 0x8C, 0x97, 0x90};
  uint8_t msg[3];
  uint16_t ret;
  float Stat;

  for (int i = 0; i < 6; i++) {
    uint8_t addy = this->get_address();
    uint8_t cs = (addy << 1) + reg[i];
    uint8_t regCS = ((0xFF - cs) + 1) & 0xFF;  // checksum

    // Begin transmission to device
    if (!this->write(reg + i, 1)) {
      ESP_LOGW(TAG, "Failed to write register 0x%02X", reg[i]);
      continue;
    }

    // Write checksum byte manually (Wire.write(regCS))
    // ESPHome I2C doesn't expose this directly, so let's write using a buffer:
    uint8_t write_buf[2] = {reg[i], regCS};
    if (!this->write(write_buf, 2)) {
      ESP_LOGW(TAG, "Failed to write reg + checksum 0x%02X", reg[i]);
      continue;
    }

    delay(1);

    // Request 3 bytes
    if (!this->read(msg, 3)) {
      ESP_LOGW(TAG, "Failed to read 3 bytes from reg 0x%02X", reg[i]);
      continue;
    }

    // Construct 16-bit value from first two bytes (LSB first)
    ret = (msg[1] << 8) + msg[0];

    switch (i) {
      case 0:  // Grid Voltage (volt_in)
        Stat = ret / 32.0f;
        ESP_LOGD(TAG, "Grid Voltage = %.2f V", Stat);
        if (vin_sensor)
          vin_sensor->publish_state(Stat);
        break;

      case 1:  // Grid Current (amp_in)
        Stat = ret / 128.0f;
        ESP_LOGD(TAG, "Grid Current = %.2f A", Stat);
        if (iin_sensor)
          iin_sensor->publish_state(Stat);
        break;

      case 2:  // Output Voltage (volt_out)
        Stat = ret / 256.0f;
        ESP_LOGD(TAG, "Output Voltage = %.2f V", Stat);
        if (vout_sensor)
          vout_sensor->publish_state(Stat);
        break;

      case 3:  // Output Current (amp_out)
        Stat = ret / 128.0f;
        ESP_LOGD(TAG, "Output Current = %.2f A", Stat);
        if (iout_sensor)
          iout_sensor->publish_state(Stat);
        break;

      case 4:  // Internal Temp (internal_Temp)
        Stat = ret / 32.0f;
        // Convert Fahrenheit to Celsius
        Stat = (Stat - 32.0f) * 5.0f / 9.0f;
        ESP_LOGD(TAG, "Internal Temp = %.2f °C", Stat);
        if (temp_sensor)
          temp_sensor->publish_state(Stat);
        break;

      case 5:  // Fan RPM (Fan_RPM)
        Stat = (float)ret;
        ESP_LOGD(TAG, "Fan RPM = %.0f", Stat);
        if (fan_sensor)
          fan_sensor->publish_state(Stat);
        break;
    }
  }
}

}  // namespace dps_1200
}  // namespace esphome
