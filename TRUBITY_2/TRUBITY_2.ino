
int sensorPin = A0;
void setup(){
Serial.begin(115200);
  
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  int turbidity = map(sensorValue, 0,640, 100, 0);
  
 
  delay(100);
  //lcd.setCursor(0, 0);
  //lcd.print("turbidity:");
  //lcd.print("   ");
  //lcd.setCursor(10, 0);
  //lcd.print(turbidity);
  delay(100);
  if (turbidity < 20) {
    
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    
    
    Serial.println(" its CLEAR ");
  
    //lcd.setCursor(0, 1);
    //lcd.print(" its CLEAR ");
  }
  if ((turbidity > 10) && (turbidity < 50)) {
    
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
    
    Serial.println(" its CLOUDY ");

    
    //lcd.setCursor(0, 1);
    //lcd.print(" its CLOUDY ");

    
  }
  if (turbidity > 50) {
    
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, HIGH);
    
    Serial.println(" its DIRTY ");
    //lcd.setCursor(0, 1);
    //lcd.print(" its DIRTY ");
    

  
 
  }

}
