#include "seeed_eink_i2c.h"
#include "esphome/core/log.h"
#include "esphome/components/deep_sleep/deep_sleep_component.h"

namespace esphome {
namespace seeed_eink_i2c {

static const char *TAG = "seeed_eink";

void SeeedEInk::setup() {
  ESP_LOGI(TAG, "Seeed I2C E-Ink initialized (battery mode)");
}

void SeeedEInk::update() {
  auto now = this->get_time();
  if (!now.is_valid()) return;

  // Clear screen
  uint8_t clear_cmd[] = {0x01};
  this->send_data_(clear_cmd, 1);
  delay(300);

  char time_str[16];
  char date_str[32];

  snprintf(time_str, sizeof(time_str), "%02d:%02d", now.hour, now.minute);
  snprintf(date_str, sizeof(date_str), "%04d-%02d-%02d",
           now.year, now.month, now.day_of_month);

  uint8_t buf[80];
  int len = snprintf((char *)buf, sizeof(buf),
                     "T,20,40,3,%s\nT,20,95,2,%s\n",
                     time_str, date_str);

  this->send_data_(buf, len);

  // Immediately go to deep sleep after refresh
  auto *ds = deep_sleep::DeepSleepComponent::get_instance();
  if (ds != nullptr) {
    ds->start_deep_sleep();
  }
}

void SeeedEInk::draw_absolute_pixel_internal(int, int, Color) {
  // Not used (text-only display)
}

void SeeedEInk::send_data_(const uint8_t *data, size_t len) {
  this->write(data, len);
}

}  // namespace seeed_eink_i2c
}  // namespace esphome
