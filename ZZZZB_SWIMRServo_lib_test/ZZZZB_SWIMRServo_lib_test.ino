#include <SWIMRServo.h>
#include <OneWire.h>

float water_temp = 0.0;
unsigned long last_time = 0;

#define left_axial_motor_pin 3

// external temp sensor object
int DS18S20_Pin = 9;

// OneWire object for external temp sensor
OneWire ds(DS18S20_Pin);

SWIMRServo left_axial_motor;

int left_axial_motor_speed = 1500;

void setup()
{
 Serial.begin(115200);
 left_axial_motor.attach(left_axial_motor_pin);
}

void loop()
{
  if( millis() - last_time > 500)
  {
    last_time = millis();
    water_temp = get_water_temp();
    Serial.println(water_temp);
  }
  if( Serial.available()) {
     char inChar = Serial.read();
     if( inChar == '5') {
       Serial.println(inChar);
       left_axial_motor.writeMicroseconds(1500);
       
    } else if( inChar == '4') {
      Serial.println(inChar);
      left_axial_motor.writeMicroseconds(1250);
      
    } else if( inChar == '6') {
      Serial.println(inChar);
      left_axial_motor.writeMicroseconds(1750);
      
    }
  }
}

float get_water_temp(){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      //Serial.println("CRC is not valid!");
      return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      //Serial.print("Device is not recognized");
      return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad

  
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }
  
  ds.reset_search();
  
  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  
  return TemperatureSum;
  
}
