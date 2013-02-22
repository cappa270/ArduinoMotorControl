const int ledPin2 = 2;
int data = 0;
int x = 0;

int pin2State = LOW;

void setup() {
    pinMode(ledPin2, OUTPUT);
    pin2State = LOW;
    Serial.begin(9600);
}

void loop() {
    if (Serial.available() > 0) {
        data = Serial.read();
        Serial.print("I received: ");
        Serial.println(data); 
        pin2State = LOW; 
    } else {
        pin2State = HIGH;
    }
    digitalWrite(ledPin2, pin2State);
    delay(1000);
}
