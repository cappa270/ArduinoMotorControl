/*Fuck Light */

const int ledPin13 = 13;
const int ledPin12 = 12;
const int ledPin11 = 11;
const int ledPin10 = 10;
const int ledPin9 = 9;
const int ledPin8 = 8;
const int ledPin7 = 7;
const int ledPin6 = 6;

int ledState13 = HIGH;
int ledState12 = HIGH;
int ledState11 = HIGH;
int ledState10 = HIGH;
int ledState9 = HIGH;
int ledState8 = HIGH;
int ledState7 = HIGH;
int ledState6 = HIGH;

void setup() {
  pinMode(ledPin13, OUTPUT);
  pinMode(ledPin12, OUTPUT);
  pinMode(ledPin11, OUTPUT);
  pinMode(ledPin10, OUTPUT);
  pinMode(ledPin9, OUTPUT);
  pinMode(ledPin8, OUTPUT);
  pinMode(ledPin7, OUTPUT);
  pinMode(ledPin6, OUTPUT);
}

void loop() {
  digitalWrite( ledPin13, ledState13);
  digitalWrite( ledPin12, ledState12);
  digitalWrite( ledPin11, ledState11);
  digitalWrite( ledPin10, ledState10);
  digitalWrite( ledPin9, ledState9);
  digitalWrite( ledPin8, ledState8);
  digitalWrite( ledPin7, ledState7);
  digitalWrite( ledPin6, ledState6);
}
