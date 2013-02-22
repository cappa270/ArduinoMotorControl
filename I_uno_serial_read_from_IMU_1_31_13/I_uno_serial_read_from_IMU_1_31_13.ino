#include <SoftwareSerial.h>
#include <Servo.h>

// software serial port definition
// This port is used to recieve and transmit from the IMU to the Arduino Uno
SoftwareSerial IMU_serial(2,4); // RX, TX

char serial_in[64];                          // char buffer used to store incoming IMU data
char find_target[4] = {'!','!','!'};         // each string coming from the IMU will be preceeded by three exclamation points '!'
char end_tag = '*';                          // each string coming from the IMU will be terminated by a star character '*'

#define motor1_pin 3
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
  IMU_serial.begin(38400);
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
    // Parsing Function for extracting IMU values from Serial buffer
    
  }
  // Motor adjustment based on IMU input
  motor_driver(stopped, armed);
}


/*********************************************************************
  Function to read IMU values
  Input: Pointer to char array
  Output: Boolean - if data retrieved, bool == true
  ********************************************************************
*/
boolean get_IMU_data(char* storage_buffer) {
  if (IMU_serial.available()>=0) {
    if (IMU_serial.find(find_target) == true) {
      IMU_serial.readBytesUntil(end_tag,storage_buffer,64);
    #if DEBUGGING == 1
      Serial.println(storage_buffer);
    #endif
    return true;
    } else {
      IMU_serial.flush();
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
    } else if (temp_armed && !temp_stopped) {
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
  delay(4000);
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
  delay(500);
  Serial.println("Stopped.");
  delay(100);
  armed = false;
  stopped = true;
}
/****************************************************************
  End Motor Module
****************************************************************/
