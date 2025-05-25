#include "sensor_platform.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dps_1200 {

static const char *const TAG = "hp_psu.sensor";

HPPSUMonitorSensor *global_instance = nullptr;

}  // namespace dps_1200
}  // namespace esphome
