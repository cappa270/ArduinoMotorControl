
const int LED = 2;
int count = 0;
int dir = 0;

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
  if ( dir == 0) {
      digitalWrite(LED, HIGH);
      delay(count);
      digitalWrite(LED, LOW);
      delay(count);
      count = count + 25;
  } else {
      digitalWrite(LED, HIGH);
      delay(count);
      digitalWrite(LED, LOW);
      delay(count);
      count = count - 25;
  }
  
  if ( count == 0) {
    dir = 0;
  } else if ( count == 500) {
    dir = 1;
  } else {
    dir = dir;
  }
  
}

