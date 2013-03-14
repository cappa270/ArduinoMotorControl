
boolean new_commands_present = false;
unsigned long  last_time = 0;
byte commands[] = {0,127,127,127,127,127,127};
void setup(){
  
Serial.begin(115200);  
}

void loop()
{
  //check_battery_voltage(danger_threshold);
  if( millis()- last_time > 300)
  {
    last_time = millis();
    Serial.println("sanity print");
    //get_IMU_data();
    //motor_driver(armed);
  }
  //Serial.println(millis());
  
  if( new_commands_present)
  {
    new_commands_present = false;
    byte length = Serial.read();
    Serial.println(length);
    if (length == 7)
    {
      Serial.println("Begin");
      for( int i = 0; i < length; i++)
      {
        while( Serial.available() < 1)
        {
          Serial.println("Waiting");
        }
        commands[i] = Serial.read();
        Serial.println(commands[i]);
      }
    }

  }
}

void serialEvent()
{
  new_commands_present = true;
}
