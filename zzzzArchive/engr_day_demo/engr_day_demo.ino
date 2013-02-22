/*
  Motor Driving Software
  Written by: Mitch Fynaardt on November 14, 2012
  
  Inputs: Serial commands from Serial Monitor
  Outputs: Analog output (PWM) signals to drive motors
*/


// the byte is used to receive the serial commands from the computer
byte command[10] = {0}; 

// motor control pins, motor1 == right motor, motor2 == left motor
// Arduino pin 10 == pin 10 on bridge
int motor1_forward = 10;
// Arduino pin 11 == pin 12 on bridge
int motor1_reverse = 11;
// Arduino pin 6 == pin 7 on bridge
int motor2_forward = 6;
// Arduino pin 5 == pin 5 on bridge
int motor2_reverse = 5;

// actual motor speed is half of total possible speed, 255
int motor_speed_demo1 = 117;
int motor_speed_demo2 = 130;


// setting pins as outputs to control motor
// begin serial communications to send commands to Arduino
void setup() {
  pinMode(motor1_forward, OUTPUT);
  pinMode(motor1_reverse, OUTPUT);
  pinMode(motor2_forward, OUTPUT);
  pinMode(motor2_reverse, OUTPUT);
  Serial.begin(115200);
}


void loop() {
  //following two lines allow the user to see what the Arduino is receiving
  if (Serial.available() > 0) {
    // input command
    // f, r, or s
    command[0] = Serial.read();
    delay(250);
    
    // printing the motor speed variables allows a second check for troubleshooting
    // Serial.println(motor_speed_demo);
  }
  
    // decision statement for how to drive each motor
    switch (command[0]) {
      
      case 'w':
        // forward
        Serial.println('f');
        Serial.println(motor_speed_demo1);
        Serial.println(motor_speed_demo2);
        analogWrite(motor1_reverse, 0);
        analogWrite(motor2_reverse, 0);
        analogWrite(motor1_forward, motor_speed_demo1);
        analogWrite(motor2_forward, motor_speed_demo2);
        break;
        
      case 's':
        // backward
        Serial.println('b');
        Serial.println(motor_speed_demo1);
        Serial.println(motor_speed_demo2);
        analogWrite(motor1_forward, 0);
        analogWrite(motor2_forward, 0);
        analogWrite(motor1_reverse, motor_speed_demo1);
        analogWrite(motor2_reverse, motor_speed_demo2);
        break;
        
      case 'd':
        // turn right
        Serial.println('r');
        Serial.println(motor_speed_demo1);
        Serial.println(motor_speed_demo2);
        analogWrite(motor1_forward, 0);
        analogWrite(motor2_reverse, 0);
        analogWrite(motor1_reverse, motor_speed_demo1);
        analogWrite(motor2_forward, motor_speed_demo2);
        break;
        
      case 'a':
        // turn left
        Serial.println('l');
        Serial.println(motor_speed_demo1);
        Serial.println(motor_speed_demo2);
        analogWrite(motor1_reverse, 0);
        analogWrite(motor2_forward, 0);
        analogWrite(motor1_forward, motor_speed_demo1);
        analogWrite(motor2_reverse, motor_speed_demo2);
        break;
        
      case 'q':
        // stop
        Serial.println('s');
        analogWrite(motor1_reverse, 0);
        analogWrite(motor2_reverse, 0);
        analogWrite(motor1_forward, 0);
        analogWrite(motor2_forward, 0);
        break;
        
      default:
        break;
    }
    
  command[0] = -1;
  
}
