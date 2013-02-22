//Master I2C code sketch

#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(38400);
  
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  
  digitalWrite(7,LOW);
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
}

void loop() {
  
  // first chunk sends the slave a message if serial port receives input
  if( Serial.available() > 0) {
    char c = Serial.read();
   
    if( c == 'B') {
       Wire.beginTransmission(5);
       Wire.write('B');
       Wire.endTransmission();
    } else if( c == 'G') {
       Wire.beginTransmission(5);
       Wire.write('G');
       Wire.endTransmission();
    } else if( c == 'N') {
       Wire.beginTransmission(5);
       Wire.write('N');
       Wire.endTransmission();
    }
    
    Wire.requestFrom(5, 1);
  
    while( Wire.available()) {
    char d = Wire.read();
    Serial.print(d);
      if( d == 'b') {
        digitalWrite(8,LOW);
        digitalWrite(9,LOW);
        digitalWrite(7,HIGH);
      } else if( d == 'g') {
        digitalWrite(7,LOW);
        digitalWrite(9,LOW);
        digitalWrite(8,HIGH);
      } else if( d == 'n') {
        digitalWrite(7,LOW);
        digitalWrite(8,LOW);
        digitalWrite(9,HIGH);
      }
    }
  }
}
