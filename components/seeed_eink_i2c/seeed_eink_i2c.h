#pragma once
#include "esphome/components/display/display.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/time/real_time_clock.h"

namespace esphome {
namespace seeed_eink_i2c {

class SeeedEInk : public display::Display, public i2c::I2CDevice {
 public:
  void setup() override;
  void update() override;

  void set_time(time::RealTimeClock *time) { this->time_ = time; }

 protected:
  time::RealTimeClock *time_{nullptr};
};

}  // namespace seeed_eink_i2c
}  // namespace esphome
