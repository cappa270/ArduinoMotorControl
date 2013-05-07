
/*********************************************************************
  Written by Mitch Fynaardt
  Date: February 1, 2013
  Last Rev: April 30, 2013
  
  This sketch utilizes serial library to capture date from
  ArduIMU V3 and prints values back to the user. The Arduino Mega also
  captures user input via a hardware serial port and calculates a
  servo motor speed.
*********************************************************************/
#include <Servo.h>
#include <DHT22.h>

// analog pin for reading water temperature
#define THERMISTORPIN A1
// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many water temperature samples to take
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor in the voltage divider
#define SERIESRESISTOR 10000

// float array for storing the parsed IMU data.
int parsed_IMU_data[3];
// String to store the incoming data from the IMU
String IMU_input_string = "";

//{error, arm, roll, pitch, yaw, x, y, z}
byte commands[] = {0,0,127,127,127,127,127,127};
float danger_threshold = 10.8;
float internal_temp;
float internal_humidity;
float water_temp;
float water_depth;
float total_voltage;

// motor pin objects
#define left_axial_motor_pin 7
#define right_axial_motor_pin 2
#define strafing_motor_pin 4
#define front_left_vertical_motor_pin 6
#define front_right_vertical_motor_pin 3
#define rear_vertical_motor_pin 5

// internal temp and humidity sensor object on pin 8
#define DHT22_PIN 8

// instance of DHT22 internal temp sensor
DHT22 myDHT22(DHT22_PIN);

// samples variable is used to compute water temp
int samples[NUMSAMPLES];

// Battery sensing pin
int batt_pin = A0;

// Servo objects to control servo motors
Servo left_axial_motor;
Servo right_axial_motor;
Servo front_left_vertical_motor;
Servo front_right_vertical_motor;
Servo rear_vertical_motor;

// Initialize motor speeds to neutral
int left_axial_motor_speed = 1500;
int right_axial_motor_speed = 1500;
int front_left_vertical_motor_speed = 1500;
int front_right_vertical_motor_speed = 1500;
int rear_vertical_motor_speed = 1500;

// Initialize direction speeds
int x_speed = 127;
int y_speed = 127;
int z_speed = 127;
int roll_speed = 127;
int pitch_speed = 127;
int yaw_speed = 127;

// loop tracking variables
unsigned long last_time = 0;
unsigned long loop_runs = 0;
unsigned long last_sensor_read = 0;

// motor state tracker
boolean armed = false;

// serial input flags
boolean new_commands_present = false;
boolean new_IMU_data_present = false;

// danger status flags
boolean water_leak = false;
boolean returning_to_surface = false;

// User defined settings
#define FRESH_WATER 1
#if FRESH_WATER == 1
  float depth_conversion = 2.9647456347;
#else
  float depth_conversion = 3.0336932076;
#endif

// used to print debugging messages
#define DEBUGGING 0

void setup()
{
  Serial.begin(115200);
  Serial1.begin(38400);
  IMU_input_string.reserve(38);
  
  boolean motor_setup_debug = motor_setup();
}

void loop()
{
  if( millis()- last_time > 50)
  {
    last_time = millis();
    // new values read from IMU
    get_IMU_data();
    // new IMU values and user input interpretted into motor commands
    motor_driver();
    // sensor values read and calculated
    dht22();
    water_temp = get_water_temp();
    water_depth = get_depth();
    // battery check function
    check_battery_voltage(danger_threshold);
    // send all updated values and info to the user
    send_data();
  }
  // function that reads and stores new user commands
  new_commands();
  // function that receives new IMU data
  new_IMU_data_received();
  
}

/*********************************************************************
  Function to read IMU values and make them useful for the Arduino MEGA
  Input: None
  Output: None
  *******************************************************************/
void get_IMU_data()
{
  /* motor driver only runs when there is new IMU input data */
  if( IMU_input_string.length() > 0)
  {
      // Next 6 statements are used to mark indeces of the delimiter characters
      int colon_1 = IMU_input_string.indexOf(':');
      int comma_1 = IMU_input_string.indexOf(',');
      
      int colon_2 = IMU_input_string.indexOf(':',colon_1 + 1);
      int comma_2 = IMU_input_string.indexOf(',',comma_1+1);
      
      int colon_3 = IMU_input_string.indexOf(':',colon_2 + 1);
      int star = IMU_input_string.indexOf('*',colon_2 + 1);
      
      String roll_string = IMU_input_string.substring(colon_1+1,comma_1);
      String pitch_string = IMU_input_string.substring(colon_2+1,comma_2);
      String yaw_string = IMU_input_string.substring(colon_3+1,star);
      
      // Conversion from strings to ints
      parsed_IMU_data[0] = roll_string.toInt();
      parsed_IMU_data[1] = pitch_string.toInt();
      parsed_IMU_data[2] = yaw_string.toInt();
      
      IMU_input_string = "";
  }  
}
/****************************************************************
  End IMU serial read module
****************************************************************/

/****************************************************************
  Motor module
  This function attaches motor pins and runs startup configurations
****************************************************************/
boolean motor_setup()
{
  left_axial_motor.attach(left_axial_motor_pin);
  right_axial_motor.attach(right_axial_motor_pin);
  front_left_vertical_motor.attach(front_left_vertical_motor_pin);
  front_right_vertical_motor.attach(front_right_vertical_motor_pin);
  rear_vertical_motor.attach(rear_vertical_motor_pin);
  
  left_axial_motor_speed = 1500;
  right_axial_motor_speed = 1500;
  front_left_vertical_motor_speed = 1500;
  front_right_vertical_motor_speed = 1500;
  rear_vertical_motor_speed = 1500;
  
  left_axial_motor.writeMicroseconds(left_axial_motor_speed);
  right_axial_motor.writeMicroseconds(right_axial_motor_speed);
  front_left_vertical_motor.writeMicroseconds(front_left_vertical_motor_speed);
  front_right_vertical_motor.writeMicroseconds(front_right_vertical_motor_speed);
  rear_vertical_motor.writeMicroseconds(rear_vertical_motor_speed);
  
  return left_axial_motor.attached() && right_axial_motor.attached() && front_left_vertical_motor.attached() && front_right_vertical_motor.attached() && rear_vertical_motor.attached();
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
void motor_driver()
{
  // storage variables which will be used to scale the motor speeds
  int current_roll = int(parsed_IMU_data[0]);
  int current_pitch = int(parsed_IMU_data[1]);
  int current_yaw = int(parsed_IMU_data[2]);
  int max_user_input = 255;
  int min_user_input = 0;
  
  // maximum increments for motors to prevent motors from drawing more than 12 Amps
  int user_roll_max = 250;
  int user_pitch_max = 250;
  int user_yaw_max = 250;
  int user_x_max = 250;
  int user_y_max = 250;
  int user_z_max = 250;
  
  // this variable is the maximum drive increment that the IMU may send to adjust position
  int max_IMU_adjustment = 250;
  
  if( armed)
  {
    // IMU calculations take place here, the results will bring the Sub back to neutral position
    // if no user input is being received
    current_roll = constrain(current_roll, -180, 180);
    current_pitch = constrain(current_pitch, -180, 180);
    current_yaw = constrain(current_yaw, -180, 180);
    
    int IMU_roll_adjustment = map(current_roll, -180, 180, -max_IMU_adjustment, max_IMU_adjustment);
    int IMU_pitch_adjustment = map(current_pitch, -180, 180, -max_IMU_adjustment, max_IMU_adjustment);
    int IMU_yaw_adjustment = map(current_yaw, -180, 180, -max_IMU_adjustment, max_IMU_adjustment);
    
    // ADD DESCRIPTORS ABOVE EACH MOTOR INDICATING WHICH DIRECTION EACH MAX VALUE DRIVES
    roll_speed = map((int)commands[2], min_user_input, max_user_input, -user_roll_max, user_roll_max);
    //
    pitch_speed = map((int)commands[3], min_user_input, max_user_input, -user_pitch_max, user_pitch_max);
    //
    yaw_speed = map((int)commands[4], min_user_input, max_user_input, -user_yaw_max, user_yaw_max);
    // 
    x_speed = map((int)commands[5], min_user_input, max_user_input, -user_x_max, user_x_max);
    // 
    y_speed = map((int)commands[6], min_user_input, max_user_input, -user_y_max, user_y_max);
    //
    z_speed = map((int)commands[7], min_user_input, max_user_input, -user_z_max, user_z_max);
    
    // Drive motors with either the user input or the IMU data
    left_axial_motor_speed = 1500 + x_speed - yaw_speed;
    right_axial_motor_speed = 1500 + x_speed + yaw_speed;
    
    if (commands[2] == 127 && commands[3] == 127)
    {
      if (current_pitch > 15 || current_pitch < -15)
      {
        front_left_vertical_motor_speed = 1500 + (0.7 * IMU_pitch_adjustment);
        front_right_vertical_motor_speed = 1500 + (0.7 * IMU_pitch_adjustment);
      } else
      {
        front_left_vertical_motor_speed = 1500;
        front_right_vertical_motor_speed = 1500;
      }
      if ( current_roll > 30 || current_roll < -30)
      {
        front_left_vertical_motor_speed = front_left_vertical_motor_speed - IMU_roll_adjustment;
        front_right_vertical_motor_speed = front_right_vertical_motor_speed + IMU_roll_adjustment;
      }
    }
    else if ( commands[2] == 127 && commands[3] != 127)
    {
      if ( current_roll > 30 || current_roll < -30)
      {
        front_left_vertical_motor_speed = 1500 + (0.7 * pitch_speed) - IMU_roll_adjustment;
        front_right_vertical_motor_speed = 1500 + (0.7 * pitch_speed) + IMU_roll_adjustment;
      } else
      {
        front_left_vertical_motor_speed = 1500 + (0.7 * pitch_speed);
        front_right_vertical_motor_speed = 1500 + (0.7 * pitch_speed);
      }
    }
    else if ( commands[2] != 127 && commands[3] == 127)
    {
      if ( current_pitch > 15 || current_pitch < -15)
      {
        front_left_vertical_motor_speed = 1500 + (0.7 * IMU_pitch_adjustment) + roll_speed;
        front_right_vertical_motor_speed = 1500 + (0.7 * IMU_pitch_adjustment) - roll_speed;
      } else
      {
        front_left_vertical_motor_speed = 1500 + roll_speed;
        front_right_vertical_motor_speed = 1500 - roll_speed;
      }
    }
    else
    {
      front_left_vertical_motor_speed = 1500 + roll_speed + (0.7 * pitch_speed);
      front_right_vertical_motor_speed = 1500 - roll_speed + (0.7 * pitch_speed);
    }
    
    front_left_vertical_motor_speed = front_left_vertical_motor_speed + (0.7 * z_speed);
    front_right_vertical_motor_speed = front_right_vertical_motor_speed + (0.7 * z_speed);
    
    if (commands[3] != 127)
    {
      rear_vertical_motor_speed = 1500 - pitch_speed + z_speed;
    }
    else if (current_pitch > 30 || current_pitch < -30)
    {
      rear_vertical_motor_speed = 1500 - IMU_pitch_adjustment + z_speed;
    } 
    else
    {
      rear_vertical_motor_speed = 1500 + z_speed;
    }
    
    left_axial_motor_speed = constrain(left_axial_motor_speed, 1250, 1750);
    right_axial_motor_speed = constrain(right_axial_motor_speed, 1250, 1750);
    front_left_vertical_motor_speed = constrain(front_left_vertical_motor_speed, 1250, 1750);
    front_right_vertical_motor_speed = constrain(front_right_vertical_motor_speed, 1250, 1750);
    rear_vertical_motor_speed = constrain(rear_vertical_motor_speed, 1250, 1750);
    
    left_axial_motor.writeMicroseconds(left_axial_motor_speed);
    right_axial_motor.writeMicroseconds(right_axial_motor_speed);
    front_left_vertical_motor.writeMicroseconds(front_left_vertical_motor_speed);
    front_right_vertical_motor.writeMicroseconds(front_right_vertical_motor_speed);
    rear_vertical_motor.writeMicroseconds(rear_vertical_motor_speed);
  }
  else
  {
    left_axial_motor_speed = 1500;
    right_axial_motor_speed = 1500;
    front_left_vertical_motor_speed = 1500;
    front_right_vertical_motor_speed = 1500;
    rear_vertical_motor_speed = 1500;
    
    left_axial_motor.writeMicroseconds(left_axial_motor_speed);
    right_axial_motor.writeMicroseconds(right_axial_motor_speed);
    front_left_vertical_motor.writeMicroseconds(front_left_vertical_motor_speed);
    front_right_vertical_motor.writeMicroseconds(front_right_vertical_motor_speed);
    rear_vertical_motor.writeMicroseconds(rear_vertical_motor_speed);
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
  left_axial_motor_speed = 1500;
  right_axial_motor_speed = 1500;
  front_left_vertical_motor_speed = 1500;
  front_right_vertical_motor_speed = 1500;
  rear_vertical_motor_speed = 1500;
    
  left_axial_motor.writeMicroseconds(left_axial_motor_speed);
  right_axial_motor.writeMicroseconds(right_axial_motor_speed);
  front_left_vertical_motor.writeMicroseconds(front_left_vertical_motor_speed);
  front_right_vertical_motor.writeMicroseconds(front_right_vertical_motor_speed);
  rear_vertical_motor.writeMicroseconds(rear_vertical_motor_speed);
  delay(20);
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
  left_axial_motor_speed = 1500;
  right_axial_motor_speed = 1500;
  front_left_vertical_motor_speed = 1500;
  front_right_vertical_motor_speed = 1500;
  rear_vertical_motor_speed = 1500;
  
  left_axial_motor.writeMicroseconds(left_axial_motor_speed);
  right_axial_motor.writeMicroseconds(right_axial_motor_speed);
  front_left_vertical_motor.writeMicroseconds(front_left_vertical_motor_speed);
  front_right_vertical_motor.writeMicroseconds(front_right_vertical_motor_speed);
  rear_vertical_motor.writeMicroseconds(rear_vertical_motor_speed);
  delay(20);
  armed = false;
}
/****************************************************************
  End Stop Module
****************************************************************/

/****************************************************************
  Serial Event Module
  Used to implement polling between the MEGA and the IMU
****************************************************************/
void serialEvent1()
{
  new_IMU_data_present = true;
}
/****************************************************************
  End Serial Event Module
****************************************************************/

/****************************************************************
  User Interface Interrupt
  Used to allow the user to drive the system
****************************************************************/
void serialEvent()
{
  new_commands_present = true;
}
/****************************************************************
  End Serial Event Module
****************************************************************/

/****************************************************************
  Battery Voltage Monitoring Module
  Used to ensure the battery does not fall below a damaging
  charge level
  INPUT: Charge threshold to check against
  OUTPUT: Boolean that is true if battery is in danger
****************************************************************/
boolean check_battery_voltage(float threshold)
{
  /***************
This function requires voltage dividers! 

Make sure that the threashold is a float!!!

This function requires A0
  ****************/
  
  // read the input on batt_pin (analog 0):
  int cell_1_raw = analogRead(batt_pin);
  
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float cell_1_voltage = ((float)cell_1_raw / 1023.0 ) * 4.98;
  // total voltage is divided by 3 in order to check raw value
  // multiplying by 3 reports the total voltage
  total_voltage = cell_1_voltage * 3.0;
  // print out the value you 
 return total_voltage <= threshold;
}
/****************************************************************
  End Battery Checking Module
****************************************************************/

/****************************************************************
  Internal Temp and Humidity Sensor Module
  Used to report the Sub internal air temp and humidity
****************************************************************/
void dht22()
{
  DHT22_ERROR_t errorCode;

  errorCode = myDHT22.readData();
  switch(errorCode)
  {
    case DHT_ERROR_NONE:
    // put values into float variables so they can be communicated to UI
      internal_temp = myDHT22.getTemperatureC();
      internal_humidity = myDHT22.getHumidity();
      if ( internal_humidity >= 90)
      {
        water_leak = true;
      }
      else {
        water_leak = false;
      }
      break;
    default:
      break;
  }
}
/****************************************************************
  End Internal Temp and Humidity Sensor Module
****************************************************************/

/****************************************************************
  External Temp Sensor Module
  Used to report the external water temperature
****************************************************************/
float get_water_temp()
{
  uint8_t i;
    float average;
   
    // take N samples in a row, with a slight delay
    for (i=0; i< NUMSAMPLES; i++)
    {
     samples[i] = analogRead(THERMISTORPIN);
     delay(5);
    }
   
    // average all the samples out
    average = 0;
    for (i=0; i< NUMSAMPLES; i++)
    {
       average += samples[i];
    }
    average /= NUMSAMPLES;
   
    // convert the value to resistance
    average = 1023 / average - 1;
    average = SERIESRESISTOR / average;
   
    float temperature;
    temperature = average / THERMISTORNOMINAL;     // (R/Ro)
    temperature = log(temperature);                // ln(R/Ro)
    temperature /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
    temperature += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
    temperature = 1.0 / temperature;               // Invert
    temperature -= 273.15;                         // convert to C
   
    return temperature;
  }
/****************************************************************
  End External Temp Sensor Module
****************************************************************/

/****************************************************************
  Water Depth Detection Module
****************************************************************/
float get_depth()
{
  /* depth is calculated by dividing the analog read value by 1023 which is
   the resolution of the Arduino. Multiplying by 0.004 and adding 10 are
   parts of the transfer function given in the sensor data sheet. Subtaracting
   101.325 kPa is needed to correct for the absolute pressure reading which includes
   atmospheric pressure. The last number is a conversion factor for feet per units 
   of pressure. This can be selected as fresh or salt water by the user. If the Sub is
   outside of the water we dont care to read the depth so the depth is constrained to zero */
  float depth = (((float)analogRead(A2) / (1023.0 * 0.004) + 10) - 98.5) / depth_conversion;
  depth = constrain(depth,0,10000); 
  return depth;
}
/****************************************************************
  End Water Depth Detection Module
****************************************************************/

/****************************************************************
  New Commands Module
  Used to read new command packets from the user
****************************************************************/
void new_commands() {
  if( new_commands_present && !returning_to_surface)
  {
    new_commands_present = false;
    byte length = Serial.read();
    if (length == 8)
    {
      for( int i = 0; i < length; i++)
      {
        while( Serial.available() < 1)
        {
        }
        commands[i] = Serial.read();
      }
      if (!armed)
      {
       commands[2] = 127;
       commands[3] = 127;
       commands[4] = 127;
       commands[5] = 127;
       commands[6] = 127;
       commands[7] = 127;
      }
      if (commands[1] == 1 && commands[0] == 0 && !armed && !water_leak) //If user wants it to arm, and there are no errors, and its not already armed
      {
        ESCArm();
      } else if (commands[1] == 0 && !water_leak ) //if the user wants it to disarm
      {
        Stop();
      } else if ( commands[0] == 1 || water_leak) //if there are errors
      {
        if(!armed) ESCArm();
        //return_to_surface();
      }
    }
  }
}
/****************************************************************
  End New Commands Module
****************************************************************/

/****************************************************************
  Send New Data Module
  Used to send new data packets to user
****************************************************************/
void send_data()
{
  float data_to_user[8];
  data_to_user[0] = parsed_IMU_data[0];
  data_to_user[1] = parsed_IMU_data[1];
  data_to_user[2] = parsed_IMU_data[2];
  data_to_user[3] = water_temp;
  data_to_user[4] = internal_temp;
  data_to_user[5] = internal_humidity;
  data_to_user[6] = water_depth;
  data_to_user[7] = total_voltage;
  
  Serial.print("$$$");
  Serial.print(data_to_user[0]);
  Serial.print(',');
  Serial.print(data_to_user[1]);
  Serial.print(',');
  Serial.print(data_to_user[2]);
  Serial.print(',');
  Serial.print(data_to_user[3]);
  Serial.print(',');
  Serial.print(data_to_user[4]);
  Serial.print(',');
  Serial.print(data_to_user[5]);
  Serial.print(',');
  Serial.print(data_to_user[6]);
  Serial.print(',');
  Serial.print(data_to_user[7]);
  Serial.print('#');

}
/****************************************************************
  End Send New Data Module
****************************************************************/

/****************************************************************
  New IMU Data Receiving Module
  Used to manage incoming IMU data
****************************************************************/
void new_IMU_data_received()
{
  if( new_IMU_data_present)
  {
    new_IMU_data_present = false;
    String valid_chars = ".,:-1234567890";
    if( IMU_input_string.length() == 0)
    {
      char in_char;
      
      do
      {
        in_char = Serial1.read();
      } while( in_char != '!');
      
      while( in_char != '*')
      {
        in_char = Serial1.read();
        
        for( int i = 0; i < valid_chars.length(); i++)
        {
          if( in_char == valid_chars[i])
          {
            IMU_input_string += in_char;
            break;
          }
        }
        if( in_char == '!')
        {
          break;
        }
      }
    }
  }
}
/****************************************************************
  End New IMU Data Receiving Module
****************************************************************/

/****************************************************************
  Return to Surface Module
  Used to bring the Sub to the surface in case of emergency
****************************************************************/
void return_to_surface()
{
  returning_to_surface = true;
  left_axial_motor.writeMicroseconds(1500);
  right_axial_motor.writeMicroseconds(1500);
  front_left_vertical_motor.writeMicroseconds(1625);
  front_right_vertical_motor.writeMicroseconds(1625);
  rear_vertical_motor.writeMicroseconds(1750);
}
/****************************************************************
  End Return to Surface Module
****************************************************************/
