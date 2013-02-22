//Slave I2C code sketch

#include <Wire.h>
char master_message;

void setup() {
  Wire.begin(5);
  
  pinMode(7,OUTPUT);   //blue
  pinMode(8,OUTPUT);   //green
  pinMode(9,OUTPUT);   //red
  digitalWrite(7,LOW);
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
  
  Wire.onReceive(getEvent);
  Wire.onRequest(requestEvent);
}

void loop() {
  delay(100);
}

void getEvent(int howMany) {
  if(Wire.available()) {
    master_message = Wire.read();
    
    if(master_message =='B') {
      digitalWrite(8,LOW);
      digitalWrite(9,LOW);
      digitalWrite(7,HIGH);
      
    } else if(master_message =='G') {
      digitalWrite(7,LOW);
      digitalWrite(9,LOW);
      digitalWrite(8,HIGH);
      
    } else {
      digitalWrite(7,LOW);
      digitalWrite(8,LOW);
      digitalWrite(9,HIGH);
    }
  }
}

void requestEvent() {
  if( master_message == 'B') {
    Wire.write('b');
  } else if( master_message == 'G') {
    Wire.write('g');
  } else if( master_message == 'N') {
    Wire.write('n');
  }
}
