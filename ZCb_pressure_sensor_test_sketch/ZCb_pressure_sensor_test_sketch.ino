unsigned long last_time = 0.0;

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  if( millis() - last_time > 1000)
  {
    last_time = millis();
    float pressure = ((float)analogRead(A2) / (1023.0 * 0.004) + 10);
    Serial.println(pressure);
  }
}
