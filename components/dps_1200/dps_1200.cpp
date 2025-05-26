#include "dps_1200.h"
#include "esphome/core/log.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace dps_1200 {

static const char *const TAG = "hp_psu";


void HPPSUMonitor::update() {
  uint8_t addy = 0x58;  // Address is already set via set_i2c_address, but kept here for clarity
  uint8_t buffer[13];

  // Read 13 bytes from register 0x00
  if (this->read_bytes(0x00, buffer, 13)) {
    float v = (buffer[0] * 256 + buffer[1]) / 100.0f;      // Voltage in Volts
    float i = (buffer[2] * 256 + buffer[3]) / 1000.0f;     // Current in Amps
    float p = (buffer[8] * 256 + buffer[9]) / 10.0f;       // Power in Watts
    float t = (buffer[10] * 256 + buffer[11]) / 10.0f;     // Temperature in Celsius

    ESP_LOGD(TAG, "Got Voltage=%.2fV, Current=%.3fA, Power=%.1fW, Temp=%.1fC", v, i, p, t);

    // Publish sensor values if sensors are configured
    if (this->vin_sensor != nullptr)
      this->vin_sensor->publish_state(v);
    if (this->iin_sensor != nullptr)
      this->iin_sensor->publish_state(i); 
    if (this->vout_sensor != nullptr)
      this->vout_sensor->publish_state(p);
    if (this->temp_sensor != nullptr)
      this->temp_sensor->publish_state(t);
  } else {
    ESP_LOGW(TAG, "Failed to read from DPS 1200 at address 0x%02X", addy);
    // Optionally publish invalid states
    if (this->vin_sensor != nullptr)
      this->vin_sensor->publish_state(NAN);
    if (this->iin_sensor != nullptr)
      this->iin_sensor->publish_state(NAN);  
    if (this->vout_sensor != nullptr)
      this->vout_sensor->publish_state(NAN);
    if (this->temp_sensor != nullptr)
      this->temp_sensor->publish_state(NAN);
  }
}

}  // namespace dps_1200
}  // namespace esphome
