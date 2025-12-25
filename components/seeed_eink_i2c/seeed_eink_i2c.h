#pragma once
#include "esphome/components/display/display.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace seeed_eink_i2c {

class SeeedEInk : public display::Display, public i2c::I2CDevice {
 public:
  void setup() override;
  void update() override;
  void draw_absolute_pixel_internal(int x, int y, Color color) override;

 protected:
  int get_width_internal() override { return 264; }
  int get_height_internal() override { return 176; }

  void send_data_(const uint8_t *data, size_t len);
};

}  // namespace seeed_eink_i2c
}  // namespace esphome
