/*
  ReadAnalogVoltage
  Reads an analog input on pin 0, converts it to voltage, and prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.
 
 */
 
 float totalvoltage = 11.1;


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
Serial.print(check_battery_voltage(10.5));
delay(500);
}

boolean check_battery_voltage(float threashold){
  /***************
This function requires voltage dividors! 

Make sure that the threashold is a float!!!

This function requires A0,A1,A2


Referring to the battery monitoring pins comming out of a standard LiPo battery:
  The voltage on the red wire needs to be divided by 3 before connecting to analog pin
  The voltage on the blue wire need to be divided in half before connecting to analog pin
  The voltage on the yellow wire can be read directly by the arduino
  The Black wire is ground

  It does not matter which color goes to A0,A1,A2
  ****************/
  
  
  // read the input on analog pin 0:
  int cell1raw = analogRead(A0);
  int cell2raw = analogRead(A1);
  int cell3raw = analogRead(A2);
  
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float cell1voltage = (cell1raw / 1023.0 ) * 5.0;
  float cell2voltage = (cell2raw / 1023.0 ) * 5.0;
  float cell3voltage = (cell3raw / 1023.0 ) * 5.0;
  float totalvoltage = cell1voltage + cell2voltage + cell3voltage;
  // print out the value you read:
 
// Serial.print("Cell 1: "); Serial.print(cell1voltage); 
// Serial.print("  Cell 2: "); Serial.print(cell2voltage); 
// Serial.print("  Cell 3: "); Serial.print(cell3voltage);
// Serial.print(" Total Voltage: "); Serial.print(totalvoltage); Serial.print('\n');
 return totalvoltage <= threashold;
}
