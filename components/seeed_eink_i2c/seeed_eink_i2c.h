#pragma once
#include "esphome/components/display/display.h"
#include "esphome/components/spi/spi.h"
#include "esphome/components/time/real_time_clock.h"

namespace esphome {
namespace seeed_eink_spi {

class SeeedEInk : public display::Display, public spi::SPIDevice {
 public:
  void setup() override;
  void update() override;

  // Required Display interface
  void draw_pixel_at(int x, int y, Color color) override;
  display::DisplayType get_display_type() override;
  int get_width_internal() override;
  int get_height_internal() override;

  void set_time(time::RealTimeClock *time) { this->time_ = time; }

 protected:
  time::RealTimeClock *time_{nullptr};
};

}  // namespace seeed_eink_spi
}  // namespace esphome
