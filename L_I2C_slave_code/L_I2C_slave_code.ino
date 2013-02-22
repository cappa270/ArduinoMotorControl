//slave code
//lights an LED based on what message is sent from the master
#include <Wire.h>
char c = '\0';

void setup() {
  Wire.begin(5);
  
  pinMode(7,OUTPUT);   //blue
  pinMode(8,OUTPUT);   //green
  pinMode(9,OUTPUT);   //red
  digitalWrite(7,LOW);
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
}

void loop() {
  Wire.onReceive(getEvent);
  delay(100);
  Wire.onRequest(requestEvent);
  delay(100);
}

void getEvent(int howMany) {
  if(Wire.available()) {
    c = Wire.read();
    
    if(c =='B') {
      digitalWrite(8,LOW);
      digitalWrite(9,LOW);
      digitalWrite(7,HIGH);
      
    } else if(c =='G') {
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
  if( c == 'B') {
    Wire.write('b');
  } else if( c == 'G') {
    Wire.write('g');
  } else if( c == 'N') {
    Wire.write('n');
  }
}
