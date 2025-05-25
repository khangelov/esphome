#include "dps1200_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dps1200_sensor {

static const char *TAG = "dps1200_sensor";

void DPS1200Sensor::setup() {
  ESP_LOGD(TAG, "Setting up DPS1200 Sensor...");
  if (!this->write(nullptr, 0)) {  // Test I2C communication
    ESP_LOGW(TAG, "I2C initialization failed!");
    this->mark_failed();
  }
}

void DPS1200Sensor::update() {
  if (this->is_failed()) {
    ESP_LOGW(TAG, "Sensor failed, skipping update.");
    return;
  }

  // Read PMBus values
  float vin = read_pmbus(0x88);  // READ_VIN
  float iin = read_pmbus(0x89);  // READ_IIN
  float vout = read_pmbus(0x8B); // READ_VOUT
  float iout = read_pmbus(0x8C); // READ_IOUT

  // Calculate power
  float pin = (vin != NAN && iin != NAN) ? vin * iin : NAN;
  float pout = (vout != NAN && iout != NAN) ? vout * iout : NAN;

  // Publish to sensors
  if (vin_sensor_ && vin != NAN) vin_sensor_->publish_state(vin);
  if (iin_sensor_ && iin != NAN) iin_sensor_->publish_state(iin);
  if (pin_sensor_ && pin != NAN) pin_sensor_->publish_state(pin);
  if (vout_sensor_ && vout != NAN) vout_sensor_->publish_state(vout);
  if (iout_sensor_ && iout != NAN) iout_sensor_->publish_state(iout);
  if (pout_sensor_ && pout != NAN) pout_sensor_->publish_state(pout);
}

float DPS1200Sensor::decode_linear11(uint16_t val) {
  int16_t exponent = (val >> 11) & 0x1F;
  if (exponent & 0x10) exponent |= 0xFFE0;  // Sign extend
  int16_t mantissa = val & 0x7FF;
  if (mantissa & 0x400) mantissa |= 0xF800;  // Sign extend
  return mantissa * pow(2, exponent);
}

float DPS1200Sensor::read_pmbus(uint8_t command) {
  uint8_t data[2];
  if (!this->write(&command, 1) || !this->read(data, 2)) {
    ESP_LOGW(TAG, "I2C read failed for command 0x%02X", command);
    return NAN;
  }
  uint16_t raw = (data[1] << 8) | data[0];
  return decode_linear11(raw);
}

}  // namespace dps1200_sensor
}  // namespace esphome