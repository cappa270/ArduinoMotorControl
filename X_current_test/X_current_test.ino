#include <Servo.h>
#define motor_pin 2

Servo motor;

void setup() {
  Serial.begin(38400);
  motor.attach(motor_pin);
  Serial.println("Open");
}

void loop() 
{
  if( Serial.available()) 
  {
     char inChar = Serial.read();
     
     switch (inChar)
     {
       case 's':
         Serial.println("Stop");
         motor.writeMicroseconds(1500);
         break;
       case '0':
         Serial.println(inChar);
         motor.writeMicroseconds(2000);
         break;
       case '1':
         Serial.println(inChar);
         motor.writeMicroseconds(1550);
         break;
       case '2':
         Serial.println(inChar);
         motor.writeMicroseconds(1600);
         break;
       case '3':
         Serial.println(inChar);
         motor.writeMicroseconds(1650);
         break;
       case '4':
         Serial.println(inChar);
         motor.writeMicroseconds(1700);
         break;
       case '5':
         Serial.println(inChar);
         motor.writeMicroseconds(1750);
         break;
       case '6':
         Serial.println(inChar);
         motor.writeMicroseconds(1760);
         break;
       case '7':
         Serial.println(inChar);
         motor.writeMicroseconds(1770);
         break;
       case '8':
         Serial.println(inChar);
         motor.writeMicroseconds(1780);
         break;
       case '9':
         Serial.println(inChar);
         motor.writeMicroseconds(1790);
         break;
     }
     
  }
}
