/*
This is not working correct. it compiles and runs, But
it is no giving readings. it is on an ESP 32 board.
Trubidty2 is running on arduino UNO board.
int sensorPin = 34;
*/


void setup() {
  Serial.begin(115200);
 
}
void loop() {
  int sensorValue = analogRead(34);
  //int turbidity = map(sensorValue, 50, 4095, 100, 0);
  int turbidity = map(sensorValue, 0, 4095, 100, 24); 
  //float voltage = sensorValue * (5.0 / 1024.0);
 

  //Serial.println ("Sensor Output (V):");
  //Serial.println (turbidity);
  //Serial.println();

  Serial.print("Raw Sensor Value: ");
Serial.println(sensorValue);
  delay(1000);
}