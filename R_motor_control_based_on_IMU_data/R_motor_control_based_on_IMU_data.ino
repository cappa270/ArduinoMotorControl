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
String roll_string = "";                       // String to fill with the roll data
String pitch_string = "";                      // String to fill with the pitch data
String yaw_string = "";                        // String to fill with the yaw data

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
unsigned long last_time = 0;
unsigned long loop_runs = 0;

boolean stopped = true;
boolean armed = false;
boolean stringComplete = false;
boolean ready_for_data = true;

#define DEBUGGING 1                          // used to print debugging messages

void setup() {
  Serial.begin(38400);
  Serial1.begin(38400);
  IMU_input_string.reserve(64);
  roll_string.reserve(10);
  pitch_string.reserve(10);
  yaw_string.reserve(10);
  
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
    get_IMU_data(parsed_IMU_data);
    // Motor adjustment based on IMU input
    motor_driver(stopped, armed, parsed_IMU_data);
    Serial1.flush();
  }
}

/*********************************************************************
  Function to read IMU values
  Input: 
  Output: 
  *******************************************************************/
void get_IMU_data(float* IMU_data) {
  // colons, commas, and the star are used to mark where each piece of data may be found
  int colon_1;
  int colon_2;
  int colon_3;
  int comma_1;
  int comma_2;
  int star;
  
  // The if statement provides a condition for running the conversion. We
  // only want to run the conversion every half second in order to not be
  // swtiching the motor speeds constantly
  if( stringComplete == true) {
      // Next 6 statements are used to mark indeces of the delimiter characters
      colon_1 = IMU_input_string.indexOf(':');
      comma_1 = IMU_input_string.indexOf(',');
      
      colon_2 = IMU_input_string.indexOf(':',colon_1 + 1);
      comma_2 = IMU_input_string.indexOf(',',comma_1+1);
      
      colon_3 = IMU_input_string.indexOf(':',colon_2 + 1);
      star = IMU_input_string.indexOf('*');
      
      roll_string = IMU_input_string.substring(colon_1+1,comma_1);
      pitch_string = IMU_input_string.substring(colon_2+1,comma_2);
      yaw_string = IMU_input_string.substring(colon_3+1,star);
      
      // Conversion from strings to floating point numbers
      IMU_data[0] = roll_string.toFloat();
      IMU_data[1] = pitch_string.toFloat();
      IMU_data[2] = yaw_string.toFloat();
      
      IMU_input_string = "";
      stringComplete = false;
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
        Serial.print(IMU_data[0]);
        Serial.print(" PCH:");
        Serial.print(IMU_data[1]);
        Serial.print(" YAW: ");
        Serial.println(IMU_data[2]);
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
  return motor1.attached() && motor2.attached();
}
/****************************************************************
  End Motor Module
****************************************************************/

/****************************************************************
  Motor Driver Module
  Input: Booleans - state of the servo motor
  Output: void
  This function starts and stops each motor according to input
****************************************************************/
void motor_driver(boolean& temp_stopped, boolean& temp_armed, float* IMU_data) {
  // base_value is our test value for the sensors
  float base_value = 0.0;
  // storage variables which will be used to scale the motor speeds
  float current_roll;
  float current_pitch;
  float current_yaw;
  
  if( Serial.available()) {
    char command = Serial.read();
    if( (command == 'A' || command == 'a') && !temp_armed) {
      ESCArm();
    } else if( (command == 'S' || command == 's') && !temp_stopped) {
      Stop();
    }
  }
  if(temp_armed && !temp_stopped) {
    // current role must be calculated to determine how much the motor
    // must compensate
    current_roll = base_value - IMU_data[0];

    int m1; 
    m1 = int(current_roll);
    
    m1 = constrain(m1, -180, 180);
    motor1_speed = map(m1, -180, 180, 1300, 1700);
    motor2_speed = map(m1, 180, -180, 1300, 1700);
    
    Serial.print("Servo Speed: ");
    Serial.print(motor1_speed);
    Serial.print(" , ");
    Serial.println(motor2_speed);
    if( motor1_speed > 1490 && motor1_speed < 1510) {
      motor1.writeMicroseconds(1500);
      motor2.writeMicroseconds(1500);
    } else {
      motor1.writeMicroseconds(motor1_speed);
      motor2.writeMicroseconds(motor2_speed);
    }
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
          stringComplete = true;
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
