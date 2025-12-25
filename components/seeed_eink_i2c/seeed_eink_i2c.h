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

  // REQUIRED by Display base class
  void draw_pixel_at(int x, int y, Color color) override;
  display::DisplayType get_display_type() override;
  int get_width_internal() override;
  int get_height_internal() override;

  void set_time(time::RealTimeClock *time) { this->time_ = time; }

 protected:
  time::RealTimeClock *time_{nullptr};
};

}  // namespace seeed_eink_i2c
}  // namespace esphome
