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
    	   Serial.print ("Grid Voltage = "); Serial.print (Stat);Serial.print ("V   "); vin_sensor->publish_state(Stat); }
 	   
	   if ( i == 1) { Stat = ret / 128 ; // scale factor
    	   Serial.print ("Grid Current = "); Serial.print (Stat);Serial.print ("A   "); iin_sensor->publish_state(Stat);}

	   if ( i == 2) { Stat = ret / 256; // scale factor
    	   Serial.print ("Output Voltage = "); Serial.print (Stat);Serial.print ("V   "); vout_sensor->publish_state(Stat);  }
	  
	   if ( i == 3) { Stat = ret / 128; // scale factor
    	   Serial.print ("Output Current = "); Serial.print (Stat);Serial.println ("A  "); iout_sensor->publish_state(Stat); }

	   if ( i == 4) { Stat = ret / 32; // scale factor
	   Serial.print ("Internal Temp = "); Stat = (Stat - 32.0f) * 5.0f / 9.0f; Serial.print (Stat);Serial.print ("C  ");temp_sensor->publish_state(Stat);}
       
	   if ( i == 5) { Stat = ret ; // scale factor = 1
    	   Serial.print ("Fan RPM = "); Serial.print (Stat);Serial.print (" RPM   ");fan_sensor->publish_state(Stat);}

}



}  // namespace dps_1200
}  // namespace esphome
