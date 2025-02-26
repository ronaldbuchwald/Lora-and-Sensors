/*NOT-WORKING YET
This file needs to be ported to an ESP 32 V3 Heltec board.
Right now it works on an ESP 8266
*/
#include <OneWire.h>
#include <DallasTemperature.h>

// All DS18B20 Sensors are connected to pin D1 on the ESP8266
#define ONE_WIRE_BUS D1

// Creating a oneWire instance(object)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire object reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

int numberOfDevices; //To store number of sensor connected

DeviceAddress tempDeviceAddress; // Variable to store a single sensor address

/*----------------------------------------------------------------*/

void setup(void) {
 
  Serial.begin(9600);
  sensors.begin();
  
  // Get the number of sensors connected to the the wire( digital pin 6)
  numberOfDevices = sensors.getDeviceCount();
  
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");

  // Loop through each sensor and print out address
  for(int i=0; i<numberOfDevices; i++) {
    
    // Search the data wire for address and store the address in "tempDeviceAddress" variable
    if(sensors.getAddress(tempDeviceAddress, i)) {
      
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
     printAddress(tempDeviceAddress);
      Serial.println();
      //delay(5000);
    } else {
            Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
     
    }
    
  }//Enf of for loop
  
}//End of void setup()

/*----------------------------------------------------------------*/

void loop(void) { 
  
  sensors.requestTemperatures(); // Send the command to get temperatures from all sensors.
  
  // Loop through each device, print out temperature one by one
  for(int i=0; i<numberOfDevices; i++) {
    
    // Search the wire for address and store the address in tempDeviceAddress
    if(sensors.getAddress(tempDeviceAddress, i)){
    
      Serial.print("Temperature from sensor number: ");
      Serial.println(i,DEC);

      // Print the temperature
      float tempC = sensors.getTempC(tempDeviceAddress); //Temperature in degree celsius
      Serial.print("Temp C: ");
      Serial.print(tempC);
      Serial.print(" Temp F: ");
      Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
    
    }   
    
  }// End of for loop
  
  delay(1000);
}

/*----------------------------------------------------------------*/

// function to print a sensor address
void printAddress(DeviceAddress deviceAddress) {
  
  for (uint8_t i = 0; i < 8; i++) {
    
    if (deviceAddress[i] < 16) 
      Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
      
  }//End of for loop
  
}