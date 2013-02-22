#include <Servo.h>

#define motor1_pin 9
boolean stopped = true;
boolean armed = false;

Servo motor1;  // create servo object to control a servo
String readString;

void setup()
{
  motor1.attach(motor1_pin);
  Serial.begin(9600);
}

void loop()
{
  
   while (Serial.available()) {
    delay(10);  
    if (Serial.available() >0) {
      char c = Serial.read();  //gets one byte from serial buffer
      readString += c; //makes the string readString
    }
  }
  
  if (readString.length() >0) {
    Serial.println(readString);  //so you can see the captured string
    if((readString == "A" || readString == "a") && !armed ) {
      ESCArm();
      readString = "";
    } else if((readString == "s" || readString == "S") && !stopped ) {
      Stop();
      readString = "";
    } else if(armed && !stopped){
      int motor1_speed;
      char carray[4]; //converting string to number
      readString.toCharArray(carray, sizeof(carray));
      motor1_speed = atoi(carray);
      motor1_speed = constrain(motor1_speed, 0, 100);
      readString = "";
      Serial.print("Speed: ");
      Serial.print(motor1_speed);
      int n = map(motor1_speed, 0, 100, 1000, 2000);
      Serial.print("\tServo Value: ");
      Serial.println(n);
      //softThrottle(n, motor1);
      motor1.writeMicroseconds(n);
    } else {
      readString = "";
    }
  }
  delay(10);
}

void ESCArm() {
  Serial.println("Arming ESCs...");
  Serial.print("ONE: ");
  motor1.writeMicroseconds(1000);
  delay(4000);
  Serial.println("Done!");
  armed = true;
  stopped = false;
}

void Stop() {
//  softThrottle(0, motor1);
  motor1.writeMicroseconds(1000);
  Serial.println("Stopped."); 
  delay(100);
  armed = false;
  stopped = true;
}

//void softThrottle(int newThrottle, Servo port) {
//  
//  int oldThrottle = motor1.read();
//  int steps = abs(newThrottle - oldThrottle)/3 ;
//  for( int i = 1; i <= steps; i++) {
//   int softSpeed =  oldThrottle + i*(newThrottle-oldThrottle)/steps;
//   softSpeed = constrain(softSpeed, 1000, 2000);
//   port.writeMicroseconds(softSpeed); 
//   Serial.print("Soft Throttle: ");
//   Serial.println(softSpeed);
//   delay(10);
//  }
//  
//}
