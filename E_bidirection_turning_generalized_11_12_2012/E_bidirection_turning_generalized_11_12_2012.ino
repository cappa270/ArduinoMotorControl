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

// throttle is the speed of the motor which that the user sends serially
// throttle_motor1 is the right motor and throttle_motor2 is the left
int throttle_motor1;
int throttle_motor2;

// actual motor speed calculated using one of the throttle variables and a map command
// motor1_speed is the right motor and motor2_speed is the left
int motor1_speed = 0;
int motor2_speed = 0;


// setting pins as outputs to control motor
// begin serial communications to send commands to Arduino
void setup() {
  pinMode(motor1_forward, OUTPUT);
  pinMode(motor1_reverse, OUTPUT);
  pinMode(motor2_forward, OUTPUT);
  pinMode(motor2_reverse, OUTPUT);
  Serial.begin(9600);
}


void loop() {
  //following two lines allow the user to see what the Arduino is receiving
  if (Serial.available() > 0) {
    // input command
    // f, r, or s
    command[0] = Serial.read();
    delay(250);
    // 1 or 0
    command[1] = Serial.read();
    delay(250);
    
    // Serial comms use chars so the next two lines transform the char
    // into an integer, -48 is the offset in the ASCII table to make
    // the throttle value equal to the  provided by the user
    throttle_motor1 = int(command[1])-48;
    throttle_motor2 = int(command[1])-48;
    
    // allows the user to confirm both throttle values
    Serial.println(throttle_motor1);
    Serial.println(throttle_motor2);
    
    // map command re-scales 0-9 across interval 0-255 so that the motor speed
    // vaiables are useful for the analog write commands
    motor1_speed = map(throttle_motor1,0,9,0,255);
    motor2_speed = map(throttle_motor2,0,9,0,255);
    
    // printing the motor speed variables allows a second check for troubleshooting
    Serial.println(motor1_speed);
    Serial.println(motor2_speed); 
  }
  
    // decision statement for how to drive each motor
    switch (command[0]) {
      
      case 'w':
        // forward
        Serial.println('f');
        analogWrite(motor1_reverse, 0);
        analogWrite(motor2_reverse, 0);
        analogWrite(motor1_forward, motor1_speed);
        analogWrite(motor2_forward, motor2_speed);
        break;
        
      case 'W':
      // forward
        Serial.println('f');
        analogWrite(motor1_reverse, 0);
        analogWrite(motor2_reverse, 0);
        analogWrite(motor1_forward, motor1_speed);
        analogWrite(motor2_forward, motor2_speed);
        break;
        
      case 's':
        // backward
        Serial.println('b');
        analogWrite(motor1_forward, 0);
        analogWrite(motor2_forward, 0);
        analogWrite(motor1_reverse, motor1_speed);
        analogWrite(motor2_reverse, motor2_speed);
        break;
        
      case 'S':  
        // backward
        Serial.println('b');
        analogWrite(motor1_forward, 0);
        analogWrite(motor2_forward, 0);
        analogWrite(motor1_reverse, motor1_speed);
        analogWrite(motor2_reverse, motor2_speed);
        break;
        
      case 'd':
        // turn right
        Serial.println('r');
        analogWrite(motor1_forward, 0);
        analogWrite(motor2_reverse, 0);
        analogWrite(motor1_reverse, motor1_speed);
        analogWrite(motor2_forward, motor2_speed);
        break;
        
      case 'D':
        // turn right
        Serial.println('r');
        analogWrite(motor1_forward, 0);
        analogWrite(motor2_reverse, 0);
        analogWrite(motor1_reverse, motor1_speed);
        analogWrite(motor2_forward, motor2_speed);
        break;
        
      case 'a':
        // turn left
        Serial.println('l');
        analogWrite(motor1_reverse, 0);
        analogWrite(motor2_forward, 0);
        analogWrite(motor1_forward, motor1_speed);
        analogWrite(motor2_reverse, motor2_speed);
        break;
        
      case 'A':
        // turn left
        Serial.println('l');
        analogWrite(motor1_reverse, 0);
        analogWrite(motor2_forward, 0);
        analogWrite(motor1_forward, motor1_speed);
        analogWrite(motor2_reverse, motor2_speed);
        break;
        
      case 'q':
        // stop
        Serial.println('s');
        analogWrite(motor1_reverse, 0);
        analogWrite(motor2_reverse, 0);
        analogWrite(motor1_forward, 0);
        analogWrite(motor2_forward, 0);
        break;
        
      case 'Q':
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
