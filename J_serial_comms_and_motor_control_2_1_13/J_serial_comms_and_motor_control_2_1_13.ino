/*********************************************************************
  Written by Mitch Fynaardt with Jeff Kloosterman and Mike Capozzoli
  Date: February 1, 2013
  
  This sketch utilizes software serial library to capture date from
  ArduIMU V3 and prints values back to the user. The Arduino Mega also
  captures user input via a hardware serial port and calculates a
  servo motor speed.
*********************************************************************/
#include <Servo.h>

char serial_in[64];                          // char buffer used to store incoming IMU data
char find_target[3] = {'!','!'};             // each string coming from the IMU will be preceeded by three exclamation points '!'
char end_tag = '*';                          // each string coming from the IMU will be terminated by a star character '*'

#define motor1_pin 2
//#define motor2_pin 5
//#define motor3_pin 6
//#define motor4_pin 9
//#define motor5_pin 10
//#define motor6_pin 11

// Servo objects to control servo motors
Servo motor1;
//Servo motor_2;
//Servo motor_3;
//Servo motor_4;
//Servo motor_5;
//Servo motor_6;

// Initialize motor speeds to neutral
int motor1_speed = 1500;

boolean stopped = true;
boolean armed = false;

#define DEBUGGING 1                          // used to print debugging messages

void setup() {
  Serial.begin(38400);
  Serial1.begin(38400);
  
  motor_setup();
  #if DEBUGGING == 1
    if (motor_setup() == true) {
      Serial.println("Motors successfully attached to drive pins");
    } else {
      Serial.println("Motor pins not attached"); 
    }
  #endif
}

void loop() {
  if (get_IMU_data(serial_in)) {
    // Call to parsing function to make the IMU
    // output meaningful to the Arduino Mega
    
  }
  // Motor adjustment based on IMU input
  motor_driver(stopped, armed);
}

/*********************************************************************
  Function to read IMU values
  Input: Pointer to char array
  Output: Boolean - if data retrieved, bool == true
  *******************************************************************/
boolean get_IMU_data(char* storage_buffer) {
  if (Serial1.available()>=0) {
    if (Serial1.find(find_target) == true) {
      Serial1.readBytesUntil(end_tag,storage_buffer,64);
    #if DEBUGGING == 1
      Serial.println(storage_buffer);
    #endif
    return true;
    } else {
      Serial1.flush();
      return false;
    }
  }
}
/****************************************************************
  End IMU serial read module
****************************************************************/

/****************************************************************
  Motor module
  This function attaches motor pins and runs startup configurations
****************************************************************/
boolean motor_setup() {
  motor1.attach(motor1_pin);
  return motor1.attached();
}
/****************************************************************
  End Motor Module
****************************************************************/

/****************************************************************
  Motor Driver Module
  Input: Booleans - state of the servo motor
  Output: Boolean - if motors updated, bool == true
  This function starts and stops each motor according to input
****************************************************************/
boolean motor_driver(boolean& temp_stopped, boolean& temp_armed) {
  if (Serial.available() > 0) {
    char driver_input = Serial.read();
    Serial.println(driver_input); // allows user to see the received char
    if ((driver_input == 'A' || driver_input == 'a') && !temp_armed) {
      ESCArm();
    } else if ((driver_input == 'S' || driver_input == 's') && !temp_stopped) {
      Stop();
    } else if (driver_input == '0') {
      Serial.println("0 is not an acceptable input");
    } else if (!temp_stopped) {
      int speed_scaling_int = int(driver_input) - int('0');
      motor1_speed = map(speed_scaling_int, 1, 9, 1000, 2000);
      Serial.println("Servo Value: ");
      Serial.println(motor1_speed);
      motor1.writeMicroseconds(motor1_speed);
    }
    return true;
  } else {
    return false;
  }
}
/****************************************************************
  End Motor Module
****************************************************************/

/****************************************************************
  ESCArm module
  This function arms the ESCs
****************************************************************/
void ESCArm() {
  Serial.println("Arming ESC...");
  motor1.writeMicroseconds(1500);
  delay(1000);
  Serial.println("Done!");
  armed = true;
  stopped = false;
}
/****************************************************************
  End Motor Module
****************************************************************/

/****************************************************************
  Stop module
  This function disarms the ESCs
****************************************************************/
void Stop() {
  Serial.println("Stopping");
  motor1.writeMicroseconds(1500);
  delay(1000);
  Serial.println("Stopped.");
  delay(100);
  armed = false;
  stopped = true;
}
/****************************************************************
  End Motor Module
****************************************************************/
