#include "dps_1200.h"
#include "esphome/core/log.h"
// Enable / Disable defines

#define ADJUST_TMP_F 18 // TODO best way to calibrate?
float intake_tmp_c = 0.0;
float internal_tmp_c = 0.0;

#ifdef ENADD_STATS
// additional vals to read
float volt_in = 0.0;
float amp_in = 0.0;
float watt_in = 0.0;
float volt_out = 0.0;
float amp_out = 0.0;
float watt_out = 0.0;
#endif


uint8_t addy = 0x58; //Supply Address verify w/I2C scanner
uint8_t i; // index for loop
uint8_t reg[6] = {0x08, 0x0a, 0x0e, 0x10, 0x1c, 0x1e}; 
uint8_t cs, regCS;
uint16_t msg[3]; //Received messages from PS
float ret, Stat; //reused calculated values

float f2c(uint16_t temp) {
  return (temp- 32) *.5556;
}


namespace esphome {
namespace dps_1200 {

static const char *const TAG = "hp_psu";

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


void HPPSUMonitor::read_and_publish(uint8_t reg, sensor::Sensor *sensor) {
  uint8_t buffer[2];
  if (this->write(&reg, 1) && this->read(buffer, 2)) {
    float val = convert_linear11(buffer[1], buffer[0]);
    sensor->publish_state(val);
  } else {
    ESP_LOGW(TAG, "Failed to read register 0x%02X", reg);
  }
}

float HPPSUMonitor::convert_linear11(uint8_t high, uint8_t low) {
  int16_t raw = (high << 8) | low;
  int8_t exponent = raw >> 11;
  int16_t mantissa = raw & 0x7FF;
  if (mantissa & 0x400) mantissa |= 0xF800;
  if (exponent & 0x10) exponent |= 0xE0;
  return mantissa * powf(2, exponent);
}

}  // namespace dps_1200
}  // namespace esphome
