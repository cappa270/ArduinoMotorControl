#include <Servo.h>
#define motor_pin 3

Servo motor;

void setup() {
  Serial.begin(38400);
  motor.attach(motor_pin);
}

void loop() {
  if( Serial.available()) {
     char inChar = Serial.read();
     if( inChar == '5') {
       Serial.println(inChar);
       motor.writeMicroseconds(1500);
    } else if( inChar == '1') {
      Serial.println(inChar);
      motor.writeMicroseconds(1000);
    } else if( inChar == '9') {
      Serial.println(inChar);
      motor.writeMicroseconds(2000);
    }
  }
}
