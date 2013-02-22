//char command[10] = "stop";
// declarations allowing pin use
byte command[10] = {0}; 
int forward_pin = 11;
int reverse_pin = 6;
int throttle;

// intermediate motor speed calculation variables
//int motor_speed0 = 0;
int motor_speed1 = 0;
int motor_speed2 = 0;
//int motor_speed3 = 0;
//int motor_speed4 = 0;

int motor_speed = 0;

// setting pins as outputs to control motor
// begin serial communications to send commands to Arduino
void setup() {
  pinMode(forward_pin, OUTPUT);
  pinMode(reverse_pin, OUTPUT);
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
//    // 0 to 9
//    command[2] = Serial.read();
//    delay(250);
//    // 0 to 9
//    command[3] = Serial.read();
//    delay(250);
//    Serial.println(command);
    
//    Serial.println(command[1]);
//    motor_speed0 = command[1]*tens_mult;
//    Serial.println(motor_speed0);
//    
//    motor_speed1 = motor_speed0 + command[2];
//    Serial.println(motor_speed1);
//    
//    motor_speed2 = motor_speed1 * .01;
//    Serial.println(motor_speed2);
//    
//    motor_speed3 = motor_speed2 * 255;
//    Serial.println(motor_speed3);

    // calculates motor speed percentage so it can be made useful as a PWM signal
//    motor_speed1 = (command[1]*100)+(command[2]*10)+command[3];
//    Serial.println(motor_speed1);
//
//    motor_speed2 = int(motor_speed);
//    Serial.println(motor_speed2);
//    motor_speed2 = char(motor_speed2);
//    Serial.println(motor_speed2);

    // converting from ascii character to decimal
//    command[1] = int(command[1])-48;
//    command[2] = int(command[2])-48;
//    command[3] = int(command[3])-48;
    
    throttle = int(command[1])-48;
    Serial.println(throttle);
    motor_speed = map(throttle,0,9,0,255);
    Serial.println(motor_speed);
//    motor_speed1 = map(motor_speed, 0, 100, 0, 255);
//    Serial.println(motor_speed);
//    Serial.println(motor_speed1);
  
//  if (incoming_byte == 102) {
//    digitalWrite(reverse_pin, LOW);
//    delay(250);
//    digitalWrite(forward_pin, HIGH);
//    
//  } else if (incoming_byte == 114) {
//    digitalWrite(forward_pin, LOW);
//    delay(250);
//    digitalWrite(reverse_pin, HIGH);
//    
//  } else if (incoming_byte == 115) {
//    digitalWrite(forward_pin, LOW);
//    digitalWrite(reverse_pin, LOW);
//  }
  
//  Serial.println(incoming_byte);
  }
  
    switch (command[0]) {
      case 'f':
        Serial.println('f');
        analogWrite(reverse_pin, 0);
        analogWrite(forward_pin, motor_speed);
        break;
      case 'r':
        Serial.println('r');
        analogWrite(forward_pin, 0);
        delay(250);
        analogWrite(reverse_pin, motor_speed);
        break;
      case 's':
        Serial.println('s');
        analogWrite(forward_pin, 0);
        analogWrite(reverse_pin, 0);
        break;
      default:
        break;
    }
  command[0] = -1;
}
