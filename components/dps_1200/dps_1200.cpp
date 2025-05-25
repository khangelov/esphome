#include "dps_1200.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dps_1200 {

static const char *const TAG = "hp_psu";

void HPPSUMonitor::update() {
  float intake_tmp_c = 0.0;
  float internal_tmp_c = 0.0;
  float volt_in = 0.0;
  float amp_in = 0.0;
  float watt_in = 0.0;
  float volt_out = 0.0;
  float amp_out = 0.0;
  float watt_out = 0.0;
  uint8_t addy = 0x58; //Supply Address verify w/I2C scanner
  uint8_t i; // index for loop
  uint8_t reg[6] = {0x08, 0x0a, 0x0e, 0x10, 0x1c, 0x1e}; 
  uint8_t cs, regCS;
  uint16_t msg[3]; //Received messages from PS
  float ret, Stat; //reused calculated values
}



}  // namespace dps_1200
}  // namespace esphome
