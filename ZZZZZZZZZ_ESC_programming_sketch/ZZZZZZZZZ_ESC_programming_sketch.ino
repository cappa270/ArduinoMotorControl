#include <Servo.h>
#define motor_pin1 3
#define motor_pin2 5
#define motor_pin3 6
#define motor_pin4 9
#define motor_pin5 10
#define motor_pin6 11

Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;
Servo motor5;
Servo motor6;

void setup() {
  Serial.begin(115200);
  motor1.attach(motor_pin1);
  motor2.attach(motor_pin2);
  motor3.attach(motor_pin3);
  motor4.attach(motor_pin4);
  motor5.attach(motor_pin5);
  motor6.attach(motor_pin6);
}

void loop() {
  if( Serial.available()) {
     char inChar = Serial.read();
     if( inChar == '5') {
       Serial.println(inChar);
       motor1.writeMicroseconds(1500);
       motor2.writeMicroseconds(1500);
       motor3.writeMicroseconds(1500);
       motor4.writeMicroseconds(1500);
       motor5.writeMicroseconds(1500);
       motor6.writeMicroseconds(1500);
    } else if( inChar == '1') {
      Serial.println(inChar);
      motor1.writeMicroseconds(1000);
      motor2.writeMicroseconds(1000);
      motor3.writeMicroseconds(1000);
      motor4.writeMicroseconds(1000);
      motor5.writeMicroseconds(1000);
      motor6.writeMicroseconds(1000);
    } else if( inChar == '9') {
      Serial.println(inChar);
      motor1.writeMicroseconds(2000);
      motor2.writeMicroseconds(2000);
      motor3.writeMicroseconds(2000);
      motor4.writeMicroseconds(2000);
      motor5.writeMicroseconds(2000);
      motor6.writeMicroseconds(2000);
    }
  }
}
