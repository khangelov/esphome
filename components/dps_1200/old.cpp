/*
void HPPSUMonitor::update3() {
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

    for (uint8_t i = 0; i < 6; i++) {
      uint8_t cs = (addy << 1) + reg[i];
      uint8_t regCS = ((0xFF - cs) + 1) & 0xFF;

	    
      Wire.beginTransmission(addy); //open i2c with dps1200
      write_byte(reg[i]); // dps1200 cycle through registers
      write_byte(regCS); // write checksum each time
       //Wire.endTransmission();  // close DPS1200 i2c //
      delay(1); // Short delay between operations
 
       Wire.requestFrom((int)addy, 3);  
       msg[0] = Wire.read(); msg[1] = Wire.read();msg[2] = Wire.read();
       ret = (msg[1] << 8) + msg[0]; // Shift to MSB + LSB
	    
      // Write register + checksum
      uint8_t write_data[2] = {reg[i], regCS};
      if (!this->write(write_data, 2)) {
        ESP_LOGW("hpps_monitor", "I2C write failed for register 0x%02X", reg[i]);
        continue;
      }

      // Read 3 bytes
      uint8_t buffer[3] = {0};
      if (!this->read_bytes(reg[i], buffer, 3)) {
        ESP_LOGW("hpps_monitor", "I2C read failed for register 0x%02X", reg[i]);
        continue;
      }

      raw = (buffer[1] << 8) | buffer[0];  // LSB + MSB

      switch (i) {
        case 0: // Input Voltage
          stat = raw / 32.0f;
          vin_sensor->publish_state(stat);
          ESP_LOGI("hpps_monitor", "Grid Voltage: %.2f V", stat);
          break;
        case 1: // Input Current
          stat = raw / 128.0f;
          iin_sensor->publish_state(stat);
          ESP_LOGI("hpps_monitor", "Grid Current: %.2f A", stat);
          break;
        case 2: // Output Voltage
          stat = raw / 256.0f;
          vout_sensor->publish_state(stat);
          ESP_LOGI("hpps_monitor", "Output Voltage: %.2f V", stat);
          break;
        case 3: // Output Current
          stat = raw / 128.0f;
          iout_sensor->publish_state(stat);
          ESP_LOGI("hpps_monitor", "Output Current: %.2f A", stat);
          break;
        case 4: // Internal Temp
          stat = raw / 32.0f;
          stat = (stat - 32.0f) * 5.0f / 9.0f;  // Convert F to C
          temp_sensor->publish_state(stat);
          ESP_LOGI("hpps_monitor", "Internal Temp: %.2f °C", stat);
          break;
        case 5: // Fan RPM
          stat = raw * 1.0f;  // scale factor = 1
          fan_sensor->publish_state(stat);
          ESP_LOGI("hpps_monitor", "Fan RPM: %.0f", stat);
          break;
      }
    }
}



void HPPSUMonitor::update1() {
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
*/
