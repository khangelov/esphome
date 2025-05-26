#include "dps_1200.h"
#include "esphome/core/log.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace dps_1200 {

static const char *const TAG = "hp_psu";

void HPPSUMonitor::setup() {
  // Set the I2C address (hardcoded to 0x40 as in your original code)
  this->set_i2c_address(0x40);
}

void HPPSUMonitor::update() {
  uint8_t addy = 0x40;  // Address is already set via set_i2c_address, but kept here for clarity
  uint8_t buffer[13];

  // Read 13 bytes from register 0x00
  if (this->read_bytes(0x00, buffer, 13)) {
    float v = (buffer[0] * 256 + buffer[1]) / 100.0f;      // Voltage in Volts
    float i = (buffer[2] * 256 + buffer[3]) / 1000.0f;     // Current in Amps
    float p = (buffer[8] * 256 + buffer[9]) / 10.0f;       // Power in Watts
    float t = (buffer[10] * 256 + buffer[11]) / 10.0f;     // Temperature in Celsius

    ESP_LOGD(TAG, "Got Voltage=%.2fV, Current=%.3fA, Power=%.1fW, Temp=%.1fC", v, i, p, t);

    // Publish sensor values if sensors are configured
    if (this->vin_sensor != nullptr)
      this->vin_sensor->publish_state(v);
    if (this->iin_sensor != nullptr)
      this->iin_sensor->publish_state(i);
    if (this->vout_sensor != nullptr)
      this->vout_sensor->publish_state(p);
    if (this->temp_sensor != nullptr)
      this->temp_sensor->publish_state(t);
  } else {
    ESP_LOGW(TAG, "Failed to read from DPS 1200 at address 0x%02X", addy);
    // Optionally publish invalid states
    if (this->vin_sensor != nullptr)
      this->vin_sensor->publish_state(NAN);
    if (this->iin_sensor != nullptr)
      this->iin_sensor->publish_state(NAN);
    if (this->power_sensor_ != nullptr)
      this->power_sensor_->publish_state(NAN);
    if (this->temp_sensor != nullptr)
      this->temp_sensor->publish_state(NAN);
  }
}

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
  uint8_t buffer[13];	

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

}

}  // namespace dps_1200
}  // namespace esphome
