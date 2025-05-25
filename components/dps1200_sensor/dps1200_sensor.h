#include "esphome.h"
#include "esphome/components/i2c/i2c.h"

class DPS1200Sensor : public Component, public I2CDevice {
 public:
  void setup() override {
    // Initialize I2C
    if (!this->wire_->begin()) {
      ESP_LOGW("DPS1200 Sensor", "I2C initialization failed!");
      this->mark_failed();
    }
  }

  void update() override {
    if (this->is_failed()) return;

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

  // Sensor setters
  void set_vin_sensor(Sensor *sensor) { vin_sensor_ = sensor; }
  void set_iin_sensor(Sensor *sensor) { iin_sensor_ = sensor; }
  void set_pin_sensor(Sensor *sensor) { pin_sensor_ = sensor; }
  void set_vout_sensor(Sensor *sensor) { vout_sensor_ = sensor; }
  void set_iout_sensor(Sensor *sensor) { iout_sensor_ = sensor; }
  void set_pout_sensor(Sensor *sensor) { pout_sensor_ = sensor; }

 private:
  Sensor *vin_sensor_ = nullptr;
  Sensor *iin_sensor_ = nullptr;
  Sensor *pin_sensor_ = nullptr;
  Sensor *vout_sensor_ = nullptr;
  Sensor *iout_sensor_ = nullptr;
  Sensor *pout_sensor_ = nullptr;

  float decode_linear11(uint16_t val) {
    int16_t exponent = (val >> 11) & 0x1F;
    if (exponent & 0x10) exponent |= 0xFFE0; // Sign extend
    int16_t mantissa = val & 0x7FF;
    if (mantissa & 0x400) mantissa |= 0xF800; // Sign extend
    return mantissa * pow(2, exponent);
  }

  float read_pmbus(uint8_t command) {
    this->wire_->beginTransmission(this->address_);
    this->wire_->write(command);
    if (this->wire_->endTransmission() != 0) {
      ESP_LOGW("DPS1200 Sensor", "I2C write failed for command 0x%02X", command);
      return NAN;
    }

    if (this->wire_->requestFrom(this->address_, (uint8_t)2) != 2) {
      ESP_LOGW("DPS1200 Sensor", "I2C read failed for command 0x%02X", command);
      return NAN;
    }

    uint8_t low = this->wire_->read();
    uint8_t high = this->wire_->read();
    uint16_t raw = (high << 8) | low;
    return decode_linear11(raw);
  }
};