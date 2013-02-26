/*********************************************************************
  Written by Mitch Fynaardt
  Date: February 1, 2013
  Last Rev: February 9, 2013
  
  This sketch utilizes serial library to capture date from
  ArduIMU V3 and prints values back to the user. The Arduino Mega also
  captures user input via a hardware serial port and calculates a
  servo motor speed.
*********************************************************************/
#include <Servo.h>

float parsed_IMU_data[3];                      // float array for storing the parsed IMU data.
String IMU_input_string = "";                  // String to store the incoming data from the IMU

// motor pin objects
#define motor1_pin 2
#define motor2_pin 3
#define motor3_pin 4
#define motor4_pin 5
#define motor5_pin 6
#define motor6_pin 7

// Servo objects to control servo motors
Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;
Servo motor5;
Servo motor6;

// Initialize motor speeds to neutral
int motor1_speed = 1500;
int motor2_speed = 1500;
int motor3_speed = 1500;
int motor4_speed = 1500;
int motor5_speed = 1500;
int motor6_speed = 1500;

// loop tracking variables
unsigned long last_time = 0;
unsigned long loop_runs = 0;

// status tracking variables
boolean stopped = true;
boolean armed = false;
boolean string_complete = false;
boolean ready_for_data = true;

#define DEBUGGING 1                          // used to print debugging messages

void setup() {
  Serial.begin(38400);
  Serial1.begin(38400);
  IMU_input_string.reserve(64);
//  roll_string.reserve(10);
//  pitch_string.reserve(10);
//  yaw_string.reserve(10);
  
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
  while( millis()- last_time > 300) {
    last_time = millis();
    // IMU data retrieved and transformed to make it useful for the Arduino
    get_IMU_data();
    // Motor adjustment based on IMU input
    motor_driver(stopped, armed);
    Serial1.flush();
  }
}

/*********************************************************************
  Function to read IMU values
  Input: 
  Output: 
  *******************************************************************/
void get_IMU_data() {
  // The if statement provides a condition for running the conversion. We
  // only want to run the conversion every half second in order to not be
  // swtiching the motor speeds constantly
  if( string_complete == true) {
      // Next 6 statements are used to mark indeces of the delimiter characters
      int colon_1 = IMU_input_string.indexOf(':');
      int comma_1 = IMU_input_string.indexOf(',');
      
      int colon_2 = IMU_input_string.indexOf(':',colon_1 + 1);
      int comma_2 = IMU_input_string.indexOf(',',comma_1+1);
      
      int colon_3 = IMU_input_string.indexOf(':',colon_2 + 1);
      int star = IMU_input_string.indexOf('*');
      
      String roll_string = IMU_input_string.substring(colon_1+1,comma_1);
      String pitch_string = IMU_input_string.substring(colon_2+1,comma_2);
      String yaw_string = IMU_input_string.substring(colon_3+1,star);
      
      // Conversion from strings to floating point numbers
      parsed_IMU_data[0] = roll_string.toFloat();
      parsed_IMU_data[1] = pitch_string.toFloat();
      parsed_IMU_data[2] = yaw_string.toFloat();
      
      IMU_input_string = "";
      string_complete = false;
      ready_for_data = true;
      
      #if DEBUGGING == 1
        Serial.println();
        Serial.println();
        Serial.print("loop run: ");
        Serial.println(loop_runs);
        loop_runs++;
        
        Serial.println("Strings");
        Serial.print("RLL:");
        Serial.print(roll_string);
        Serial.print(" PCH:");
        Serial.print(pitch_string);
        Serial.print(" YAW: ");
        Serial.println(yaw_string);
        Serial.println();
        
        Serial.println("Floats");
        Serial.print("RLL:");
        Serial.print(parsed_IMU_data[0]);
        Serial.print(" PCH:");
        Serial.print(parsed_IMU_data[1]);
        Serial.print(" YAW: ");
        Serial.println(parsed_IMU_data[2]);
      #endif
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
  motor2.attach(motor2_pin);
  motor3.attach(motor3_pin);
  motor4.attach(motor4_pin);
  motor5.attach(motor5_pin);
  motor6.attach(motor6_pin);
  return motor1.attached() && motor2.attached() && motor3.attached() && motor4.attached() && motor5.attached() && motor6.attached();
}
/****************************************************************
  End Motor Module
****************************************************************/

/****************************************************************
  Motor Driver Module
  Input: Booleans - states of the servo motors
  Output: void
  This function starts and stops each motor according to input
****************************************************************/
void motor_driver(boolean& temp_stopped, boolean& temp_armed) {
  // storage variables which will be used to scale the motor speeds
  float current_roll;
  float current_pitch;
  float current_yaw;
  char commands[6];
  
  // temp calibration variables
  int roll_max = 200;
  int pitch_max = 200;
  int yaw_max = 200;
  int x_max = 200;
  int y_max = 500;
  int z_max = 200;
  
  if( Serial.available()) {
    char command = Serial.read();
    if( (command == 'A' || command == 'a') && !temp_armed) {
      ESCArm();
    } else if( (command == 'S' || command == 's') && !temp_stopped) {
      Stop();
    }
  }
  if(temp_armed && !temp_stopped) {
    if( Serial.available()) {
      // Order of commands is roll, pitch, yaw, x - +forward/-reverse, y - +left/-right, z - +up/-down
      Serial.readBytes(commands, 6);
      Serial.println(commands[3]);
      delay(500);
      
      //
      int roll_speed = map(commands[0], 0, 255, -roll_max, roll_max);
      //
      int pitch_speed = map(commands[1], 0, 255, -pitch_max, pitch_max);
      //
      int yaw_speed = map(commands[2], 0, 255, -yaw_max, yaw_max);
      // 0 is full reverse, 255 is full forward
      int x_speed = map(commands[3], 0, 255, -x_max, x_max);
      // 0 is full right, 255 is full left
      int y_speed = map(commands[4], 0, 255, -y_max, y_max);
      //
      int z_speed = map(commands[5], 0, 255, -z_max, z_max);
      
      motor1_speed = 1500 + x_speed - yaw_speed;
      motor2_speed = 1500 + x_speed + yaw_speed;
      motor3_speed = 1500 + y_speed;
      motor4_speed = 1500 ;
      motor5_speed = 1500 ;
      motor6_speed = 1500 ;
      
      motor1.writeMicroseconds(motor1_speed);
      motor2.writeMicroseconds(motor2_speed);
      motor3.writeMicroseconds(motor3_speed);
      motor4.writeMicroseconds(motor4_speed);
      motor5.writeMicroseconds(motor5_speed);
      motor6.writeMicroseconds(motor6_speed);
      
    }
    
//    // current role must be calculated to determine how much motors
//    // 4 and 5 must compensate
//    current_roll = parsed_IMU_data[0];
//    current_pitch = parsed_IMU_data[1];
//    
//    // INSERT STATEMENT HERE THAT ADDS USER INPUT VARIABLE
//    // AND ADJUSTS THE ROLL VALUES APPROPRIATELY
//    
//
//    // roll must be converted to an integer in order to be used in
//    // a map command
//    int current_roll_integer = int(current_roll);
//    int current_pitch_integer = int(current_pitch);
//    
//    // for safety and because unpredictable inputs are always a 
//    // factor the roll integer is constrained to a usable range
//    current_roll_integer = constrain(current_roll_integer, -180, 180);
//    current_pitch_integer = constrain(current_pitch_integer, -180, 180);
//    
//    // roll values get mapped to usable ranges for the Servos
//    motor1_speed = map(current_roll_integer, -180, 180, 1300, 1700);
//    motor2_speed = map(current_roll_integer, 180, -180, 1300, 1700);
//    
//    Serial.print("Servo Speeds: ");
//    Serial.print(motor1_speed);
//    Serial.print(" , ");
//    Serial.println(motor2_speed);
//    
//    // if statement used to create a dead zone so that the sub
//    // doesn't continuously bounce back and forth
//    if( motor1_speed > 1490 && motor1_speed < 1510) {
//      motor1.writeMicroseconds(1500);
//      motor2.writeMicroseconds(1500);
//    } else {
//      motor1.writeMicroseconds(motor1_speed);
//      motor2.writeMicroseconds(motor2_speed);
//    }
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
  End Stop Module
****************************************************************/

/****************************************************************
  Serial Event Module
  Used to implement polling between the MEGA and the IMU
****************************************************************/
void serialEvent1() {
  if( ready_for_data == true) {
    if( Serial1.available()) {
      char first_char;
      do {
        first_char = Serial1.read();
      } while( first_char != '!');
      while( first_char == '!' && first_char != '*') {
        char in_char = Serial1.read();
        IMU_input_string += in_char;
        if (in_char == '*') {
          string_complete = true;
          ready_for_data = false;
          first_char = '*';
        }
      }
    }
  }
}
/****************************************************************
  End Serial Event Module
****************************************************************/
