#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace dps_1200 {

class HPPSUMonitor : public PollingComponent, public i2c::I2CDevice {
 public:
  sensor::Sensor *vin_sensor{nullptr};
  sensor::Sensor *vout_sensor{nullptr};
  sensor::Sensor *iin_sensor{nullptr};
  sensor::Sensor *iout_sensor{nullptr};
  sensor::Sensor *pin_sensor{nullptr};
  sensor::Sensor *pout_sensor{nullptr};
  sensor::Sensor *temp_sensor{nullptr};
  sensor::Sensor *fan_sensor{nullptr};

  void setup() override {}
  void update() override;

 protected:
  void read_and_publish(uint8_t reg, sensor::Sensor *sensor);
  float convert_linear11(uint8_t high, uint8_t low);
};
 void setup() override {
    // This will be called by App.setup()
 }

  void update() override {

	for (i = 0; i<6; i++) {
 	   cs = (addy<<1)+ reg[i];
 	   regCS = ((0xff - cs) +1 ) & 0xff; //calculate checksum of add+reg
  	   Wire.beginTransmission(addy); //open i2c with dps1200
  	   Wire.write(reg[i]); // dps1200 cycle through registers
  	   Wire.write(regCS); // write checksum each time
  	   Wire.endTransmission();  // close DPS1200 i2c
  	   delay(1); // Short delay between operations
 
  	   Wire.requestFrom((int)addy, 3);  
  	   msg[0] = Wire.read(); msg[1] = Wire.read();msg[2] = Wire.read();
	   ret = (msg[1] << 8) + msg[0]; // Shift to MSB + LSB
	  
	   if ( i == 0)  { Stat = ret / 32 ; // scale factor
    	   Serial.print ("Grid Voltage = "); Serial.print (Stat);Serial.print ("V   "); volt_in->publish_state(Stat); }
 	   
	   if ( i == 1) { Stat = ret / 128 ; // scale factor
    	   Serial.print ("Grid Current = "); Serial.print (Stat);Serial.print ("A   "); amp_in->publish_state(Stat);}

	   if ( i == 2) { Stat = ret / 256; // scale factor
    	   Serial.print ("Output Voltage = "); Serial.print (Stat);Serial.print ("V   "); volt_out->publish_state(Stat);  }
	  
	   if ( i == 3) { Stat = ret / 128; // scale factor
    	   Serial.print ("Output Current = "); Serial.print (Stat);Serial.println ("A  "); amp_out->publish_state(Stat); }

	   if ( i == 4) { Stat = ret / 32; // scale factor
    	   Serial.print ("Internal Temp = "); Serial.print (Stat);Serial.print ("F  ");internal_Temp->publish_state(f2c(Stat));}
       
	   if ( i == 5) { Stat = ret ; // scale factor = 1
    	   Serial.print ("Fan RPM = "); Serial.print (Stat);Serial.print (" RPM   ");Fan_RPM->publish_state(Stat);}

	}
  };



}  // namespace dps_1200
}  // namespace esphome
