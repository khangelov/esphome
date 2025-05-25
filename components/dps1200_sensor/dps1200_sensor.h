#pragma once

#include "esphome/core/component.h"
#include "sensor.h"

namespace esphome {
namespace dps1200_sensor {

class dps1200Sensor : public sensor::Sensor, public PollingComponent {
  public:  
    void set_sensor1(sensor::Sensor *sensor1) { sensor1_ = sensor1; }
    void set_sensor2(sensor::Sensor *sensor2) { sensor2_ = sensor2; }
    void set_sensor3(sensor::Sensor *sensor3) { sensor3_ = sensor3; }

    void setup() override;
    void loop() override;
    void update() override;
    void dump_config() override;

  protected:
    sensor::Sensor *sensor1_;
    sensor::Sensor *sensor2_;
    sensor::Sensor *sensor3_;
};

} //namespace dps1200_sensor
} //namespace esphome