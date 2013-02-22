
float a[2];
String data = "35.00";
String data1 = "-35.00";
unsigned long last_time = 0;

void setup() {
  Serial.begin(38400);
}

void loop() {
  while (millis() - last_time > 2000) {
    
    a[0] = data.toFloat();
    a[1] = data1.toFloat();
    
    Serial.println(data);
    Serial.println(a[0]);
    Serial.println();
    Serial.println(data1);
    Serial.println(a[1]);
    Serial.println();
    Serial.println();
    last_time = millis();
  }
}
