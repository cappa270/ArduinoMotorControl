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

//float parsed_IMU_data[3];                      // float array for storing the parsed IMU data.
//String IMU_input_string = "";                  // String to store the incoming data from the IMU
//byte commands[7] = {0,128,128,128,128,128,128};

// motor pin objects
#define motor1_pin 2
#define motor2_pin 3
//#define motor3_pin 4
//#define motor4_pin 5
//#define motor5_pin 6
//#define motor6_pin 7

// Servo objects to control servo motors
Servo motor1;
Servo motor2;
//Servo motor3;
//Servo motor4;
//Servo motor5;
//Servo motor6;

// Initialize motor speeds to neutral
int motor1_speed = 1500;  // Left axial motor

int motor2_speed = 1500;  // Right axial motor

//int motor3_speed = 1500;  // Strafing / Y axis motor
//
//int motor4_speed = 1500;  // Front and Left vertical motor
//
//int motor5_speed = 1500;  // Front and Right vertical motor
//
//int motor6_speed = 1500;  // Rear vertical motor

// loop tracking variables
//unsigned long last_time = 0;
//unsigned long loop_runs = 0;

// motor state tracker
boolean armed = false;

#define DEBUGGING 1                          // used to print debugging messages

void setup()
{
  Serial.begin(38400);
  Serial1.begin(38400);
//  IMU_input_string.reserve(38);
//  roll_string.reserve(10);
//  pitch_string.reserve(10);
//  yaw_string.reserve(10);
  
  motor_setup();
  #if DEBUGGING == 1
    if (motor_setup() == true)
    {
      Serial.println("Motors successfully attached to drive pins");
    } else {
      Serial.println("Motor pins not attached"); 
    }
  #endif
//  ESCArm();
}

void loop()
{
//  while( millis()- last_time > 300)
//  {
    //last_time = millis();
    //get_IMU_data();
    motor_driver( armed);
    //Serial1.flush();
  //}
  //Serial.print('?');
}

/*********************************************************************
  Function to read IMU values and make them useful for the Arduino MEGA
  Input: None
  Output: None
  *******************************************************************/
//void get_IMU_data()
//{
//  /* The if statement provides a condition for running the conversion. We
//     only want to run the conversion every half second in order to not be
//     swtiching the motor speeds constantly */
//  if( IMU_input_string.length() > 0)
//  {
//      // Next 6 statements are used to mark indeces of the delimiter characters
//      int colon_1 = IMU_input_string.indexOf(':');
//      int comma_1 = IMU_input_string.indexOf(',');
//      
//      int colon_2 = IMU_input_string.indexOf(':',colon_1 + 1);
//      int comma_2 = IMU_input_string.indexOf(',',comma_1+1);
//      
//      int colon_3 = IMU_input_string.indexOf(':',colon_2 + 1);
//      int star = IMU_input_string.indexOf('*',colon_2 + 1);
//      
//      String roll_string = IMU_input_string.substring(colon_1+1,comma_1);
//      String pitch_string = IMU_input_string.substring(colon_2+1,comma_2);
//      String yaw_string = IMU_input_string.substring(colon_3+1,star);
//      
//      // Conversion from strings to floating point numbers
//      parsed_IMU_data[0] = roll_string.toFloat();
//      parsed_IMU_data[1] = pitch_string.toFloat();
//      parsed_IMU_data[2] = yaw_string.toFloat();
//      
//      IMU_input_string = "";
//      
//      #if DEBUGGING == 1
//        Serial.println();
//        Serial.println();
//        Serial.print("loop run: ");
//        Serial.println(loop_runs);
//        loop_runs++;
//        
//        Serial.println("Strings");
//        Serial.print("RLL:");
//        Serial.print(roll_string);
//        Serial.print(" PCH:");
//        Serial.print(pitch_string);
//        Serial.print(" YAW: ");
//        Serial.println(yaw_string);
//        Serial.println();
//        
//        Serial.println("Floats");
//        Serial.print("RLL:");
//        Serial.print(parsed_IMU_data[0]);
//        Serial.print(" PCH:");
//        Serial.print(parsed_IMU_data[1]);
//        Serial.print(" YAW: ");
//        Serial.println(parsed_IMU_data[2]);
//      #endif
//  }  
//}
/****************************************************************
  End IMU serial read module
****************************************************************/

/****************************************************************
  Motor module
  This function attaches motor pins and runs startup configurations
****************************************************************/
boolean motor_setup()
{
  motor1.attach(motor1_pin);
  motor2.attach(motor2_pin);
//  motor3.attach(motor3_pin);
//  motor4.attach(motor4_pin);
//  motor5.attach(motor5_pin);
//  motor6.attach(motor6_pin);
  return motor1.attached() && motor2.attached(); //&& motor3.attached() && motor4.attached() && motor5.attached() && motor6.attached();
}
/****************************************************************
  End Motor Module
****************************************************************/

/****************************************************************
  Motor Driver Module
  Input: Boolean - armed state of the servo motors
  Output: void
  This function starts and stops each motor according to input
  and IMU readings
****************************************************************/
void motor_driver( boolean& temp_armed)
{
  // storage variables which will be used to scale the motor speeds
//  float current_roll;
//  float current_pitch;
//  float current_yaw;
//  int max_user_input = 255;
//  int min_user_input = 0;
  
  // temp calibration variables
//  int roll_max = 200;
//  int pitch_max = 200;
//  int yaw_max = 200;
//  int x_max = 200;
//  int y_max = 500;
//  int z_max = 200;

    if( Serial.available())
    {
      char in_char = Serial.read();
      
      switch (in_char)
      {
        case 'a':
        case 'A':
          ESCArm();
          break;
          
        case 's':
        case 'S':
          Stop();
          break;
          
        case 'i':
        case 'I':
          motor1_speed = 1600;
          motor2_speed = 1600;
          motor1.writeMicroseconds(motor1_speed);
          motor2.writeMicroseconds(motor2_speed);
          Serial.println("Forward");
          break;
          
        case 'k':
        case 'K':
          motor1_speed = 1400;
          motor2_speed = 1400;
          motor1.writeMicroseconds(motor1_speed);
          motor2.writeMicroseconds(motor2_speed);
          Serial.println("Reverse");
          break;
          
        case 'l':
        case 'L':
          motor1_speed = 1400;
          motor2_speed = 1600;
          motor1.writeMicroseconds(motor1_speed);
          motor2.writeMicroseconds(motor2_speed);
          Serial.println("Right");
          break;
          
        case 'j':
        case 'J':
          motor1_speed = 1600;
          motor2_speed = 1400;
          motor1.writeMicroseconds(motor1_speed);
          motor2.writeMicroseconds(motor2_speed);
          Serial.println("Left");
          break;
          
        case 'n':
        case 'N':
          motor1_speed = 1500;
          motor2_speed = 1500;
          motor1.writeMicroseconds(motor1_speed);
          motor2.writeMicroseconds(motor2_speed);
          Serial.println("Neutral");
          break;
        
      }
    // ADD DESCRIPTORS ABOVE EACH MOTOR INDICATING WHICH DIRECTION EACH MAX VALUES DRIVES
//    int roll_speed = map(int(commands[1]), min_user_input, max_user_input, -roll_max, roll_max);
//    //
//    int pitch_speed = map(int(commands[2]), min_user_input, max_user_input, -pitch_max, pitch_max);
//    //
//    int yaw_speed = map(int(commands[3]), min_user_input, max_user_input, -yaw_max, yaw_max);
//    // 
//    int x_speed = map(int(commands[4]), min_user_input, max_user_input, -x_max, x_max);
//    // 
//    int y_speed = map(int(commands[5]), min_user_input, max_user_input, -y_max, y_max);
//    //
//    int z_speed = map(int(commands[6]), min_user_input, max_user_input, -z_max, z_max);
    
//    motor1_speed = 1500 + x_speed - yaw_speed;
//    motor2_speed = 1500 + x_speed + yaw_speed;
//    motor3_speed = 1500 + y_speed;
//    motor4_speed = 1500 + (0.5 * pitch_speed) + z_speed;
//    motor5_speed = 1500 + (0.5 * pitch_speed) + z_speed;
//    motor6_speed = 1500 - pitch_speed + z_speed;

    
//    motor1.writeMicroseconds(motor1_speed);
//    motor2.writeMicroseconds(motor2_speed);
//    motor3.writeMicroseconds(motor3_speed);
//    motor4.writeMicroseconds(motor4_speed);
//    motor5.writeMicroseconds(motor5_speed);
//    motor6.writeMicroseconds(motor6_speed);
      
    
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
void ESCArm()
{
  Serial.println("Arming ESC...");
  motor1.writeMicroseconds(1500);
  motor2.writeMicroseconds(1500);
  delay(1000);
  Serial.println("Done!");
  armed = true;
}
/****************************************************************
  End Motor Module
****************************************************************/

/****************************************************************
  Stop module
  This function disarms the ESCs
****************************************************************/
void Stop()
{
  Serial.println("Stopping");
  motor1.writeMicroseconds(1500);
  motor2.writeMicroseconds(1500);
  delay(1000);
  Serial.println("Stopped.");
  delay(100);
  armed = false;
}
/****************************************************************
  End Stop Module
****************************************************************/

/****************************************************************
  Serial Event Module
  Used to implement polling between the MEGA and the IMU
****************************************************************/
//void serialEvent1()
//{
//  String valid_chars = ".,:-1234567890";
//  if( IMU_input_string.length() == 0)
//  {
//    char in_char;
//    
//    do
//    {
//      in_char = Serial1.read();
//    } while( in_char != '!');
//    
//    while( in_char != '*')
//    {
//      in_char = Serial1.read();
//      
//      for( int i = 0; i < valid_chars.length(); i++)
//      {
//        if( in_char == valid_chars[i])
//        {
//          IMU_input_string += in_char;
//          break;
//        }
//      }
//      if( in_char == '!')
//      {
//        break;
//      }
//    }
//  }
//}
/****************************************************************
  End Serial Event Module
****************************************************************/

/****************************************************************
  User Interface Interrupt
  Used to allow the user to drive the system
****************************************************************/
//void serialEvent()
//{
//  int length = Serial.read();
//  for( int i = 0; i < length; i++)
//  {
//    commands[i] = Serial.read();
//  }
//}
/****************************************************************
  End Serial Event Module
****************************************************************/
