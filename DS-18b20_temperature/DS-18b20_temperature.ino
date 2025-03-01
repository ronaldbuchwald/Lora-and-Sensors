/*
Now working on heltec esp-32 v3.
some simple changes and found the correct i/o port.(21)
also cleaned up the code. deleted "Dec" from all lines in code
*/


#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 21  // Change to the correct GPIO pin on Heltec ESP32

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int numberOfDevices;
DeviceAddress tempDeviceAddress;

void setup() {
  Serial.begin(115200);
  
  sensors.begin();
  numberOfDevices = sensors.getDeviceCount();
  
  Serial.print(numberOfDevices);
  Serial.println(" devices found.");

  for (int i = 0; i < numberOfDevices; i++) {
    if (sensors.getAddress(tempDeviceAddress, i)) {
      Serial.print("Found device ");
      Serial.print(i);
      Serial.print(" with address: ");
      printAddress(tempDeviceAddress);
      Serial.println();
    } else {
      Serial.print("Found ghost device at ");
      Serial.print(i);
      Serial.println(" but could not detect address. Check wiring.");
    }
  }
}

void loop() {
  sensors.requestTemperatures();
  
  for (int i = 0; i < numberOfDevices; i++) {
    if (sensors.getAddress(tempDeviceAddress, i)) {
      Serial.print("Temperature from sensor number: ");
      Serial.println(i);

      float tempC = sensors.getTempC(tempDeviceAddress);
      Serial.print("Temp C: ");
      Serial.print(tempC);
      Serial.print(" Temp F: ");
      Serial.println(DallasTemperature::toFahrenheit(tempC));
    }
  }
  
  delay(1000);
}

void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
