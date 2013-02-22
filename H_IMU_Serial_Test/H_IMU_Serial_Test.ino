//64 characters is the most that the buffer can hold
char serial_in[63];
char end_tag = '*';
#define RX 0

void setup(){
  Serial.begin(38400);
  pinMode(RX, INPUT);
}

void loop() {
  if(Serial.available()>=38) {
    Serial.readBytesUntil(end_tag,serial_in,64);
    Serial.println(serial_in);
  }
}
