//char command[10] = "stop";
int incoming_byte = 0;
int forward_pin = 11;
int reverse_pin = 6;

void setup() {
  pinMode(forward_pin, OUTPUT);
  pinMode(reverse_pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  incoming_byte = Serial.read();
  Serial.println(incoming_byte);
    
  if (incoming_byte == 102) {
    digitalWrite(reverse_pin, LOW);
    delay(250);
    digitalWrite(forward_pin, HIGH);
    
  } else if (incoming_byte == 114) {
    digitalWrite(forward_pin, LOW);
    delay(250);
    digitalWrite(reverse_pin, HIGH);
    
  } else if (incoming_byte == 115) {
    digitalWrite(forward_pin, LOW);
    digitalWrite(reverse_pin, LOW);
  }
  
//  Serial.println(incoming_byte);
//  switch (incoming_byte) {
//    case 'f':
//      digitalWrite(reverse_pin, LOW);
//      delay(250);
//      digitalWrite(forward_pin, HIGH);
//      break;
//    case 'r':
//      digitalWrite(forward_pin, LOW);
//      delay(250);
//      digitalWrite(reverse_pin, HIGH);
//      break;
//    case 's':
//      digitalWrite(forward_pin, LOW);
//      digitalWrite(reverse_pin, LOW);
//      delay(250);
//      break;
//    default:
//      digitalWrite(forward_pin, forward_pin);
//      digitalWrite(reverse_pin, reverse_pin);
//  }
  
}
