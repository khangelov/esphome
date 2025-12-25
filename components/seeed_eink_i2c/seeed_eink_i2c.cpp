#include "seeed_eink_i2c.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

namespace esphome {
namespace seeed_eink_i2c {

static const char *TAG = "seeed_eink";

void SeeedEInk::setup() {
  ESP_LOGI(TAG, "Seeed I2C E-Ink initialized");
}

void SeeedEInk::update() {
  if (this->time_ == nullptr) return;

  auto now = this->time_->now();
  if (!now.is_valid()) return;

  // Night mode: 22:30 → 04:00 (no redraw)
  bool night =
      (now.hour > 22 || (now.hour == 22 && now.minute >= 30)) ||
      (now.hour < 4);

  if (!night) {
    uint8_t clear_cmd[] = {0x01};
    this->write(clear_cmd, 1);
    delay(300);

    char time_str[16];
    char date_str[32];

    snprintf(time_str, sizeof(time_str), "%02d:%02d",
             now.hour, now.minute);
    snprintf(date_str, sizeof(date_str), "%04d-%02d-%02d",
             now.year, now.month, now.day_of_month);

    uint8_t buf[80];
    int len = snprintf((char *)buf, sizeof(buf),
                       "T,20,40,3,%s\nT,20,95,2,%s\n",
                       time_str, date_str);

    this->write(buf, len);
  }

  // Go back to deep sleep
  App.safe_reboot();
}

/* ======================================================
 * Required Display interface methods (NO-OP / CONSTANT)
 * ====================================================== */

void SeeedEInk::draw_pixel_at(int, int, Color) {
  // Not supported – text-only display
}

display::DisplayType SeeedEInk::get_display_type() {
  return display::DisplayType::DISPLAY_TYPE_BINARY;
}

int SeeedEInk::get_width_internal() {
  return 264;  // Seeed 2.7"
}

int SeeedEInk::get_height_internal() {
  return 176;  // Seeed 2.7"
}

}  // namespace seeed_eink_i2c
}  // namespace esphome
