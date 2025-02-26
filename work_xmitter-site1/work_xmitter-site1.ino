/*
This is the re-worked sketch
of the working transmitter
with sensor name and location
sending distance.
this need to be updated so we can xmitt all the sensors
I am not sure how to proceed.
*/


#include "Arduino.h"
#include <NewPing.h>
#include <SPI.h>
#include <Wire.h>
#include "HT_SSD1306Wire.h"
#include "LoRaWan_APP.h"

SSD1306Wire factory_display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED);  // addr, freq, i2c group, resolution, rst

void VextON(void) {
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
}

void VextOFF(void)  // Vext default OFF
{
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, HIGH);
}

#define trigPin 46
#define echoPin 45
#define MAX_DISTANCE 400
NewPing sonar(trigPin, echoPin, MAX_DISTANCE);

#define RF_FREQUENCY 915000000  // Hz
#define TX_OUTPUT_POWER 20      // dBm

#define LORA_BANDWIDTH 0         // [0: 125 kHz, 1: 250 kHz, 2: 500 kHz, 3: Reserved]
#define LORA_SPREADING_FACTOR 7  // [SF7..SF12]
#define LORA_CODINGRATE 1        // [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
#define LORA_PREAMBLE_LENGTH 8   // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT 0    // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON false

#define RX_TIMEOUT_VALUE 1000
#define BUFFER_SIZE 50  // Adjusted for potential larger messages

char txpacket[BUFFER_SIZE];

bool lora_idle = true;
static RadioEvents_t RadioEvents;

void OnTxDone(void);
void OnTxTimeout(void);

void setup() {
  Serial.begin(115200);
  Mcu.begin();
  VextON();
  delay(100);
  factory_display.init();
  factory_display.clear();
  factory_display.display();

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  delay(5000);
  digitalWrite(LED, LOW);

  RadioEvents.TxDone = OnTxDone;
  RadioEvents.TxTimeout = OnTxTimeout;

  Radio.Init(&RadioEvents);
  Radio.SetChannel(RF_FREQUENCY);
  Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                    LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                    LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                    true, 0, 0, LORA_IQ_INVERSION_ON, 3000);
}

// Source and destination addresses
byte localAddress = 0xBB;  // address of this device (must be unique, 0xAA or 0xBB)
byte destination = 0xFF;   // destination to send to (controller = 0x01)

// Transmitter ID
byte ID = 0xFF;  // Unique ID for this transmitter

void loop() {
  if (lora_idle == true) {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // Code for distance measurement
    long duration, distance;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration * 0.034 / 2);  // Calculate distance in cm

    Serial.print("ID:");
    Serial.print(ID);
    Serial.print(" |Distance:");
    Serial.print(distance);
    Serial.println("cm");

    // Display distance and ID on OLED
    String displayString = "Distance  " + String(distance) + "cm " ;
    String displayString1 = "HC-SR04 SENSOR:";
    String displayString2 = "SITE #1";
    



    factory_display.clear();
    factory_display.setFont(ArialMT_Plain_10);
    factory_display.drawString(0, 0, displayString);
    factory_display.setFont(ArialMT_Plain_10);
    factory_display.drawString(0, 18, displayString1);
    factory_display.drawString(0, 35, displayString2);
    
    factory_display.display();
    delay(500);
   

    // **Prepare the packet**
    snprintf(txpacket, BUFFER_SIZE, "Distance:%d cm   HC-SR04         SITE #1", distance);

    Serial.print("Sending packet: ");
    Serial.println(txpacket);

    // **Print Packet Length**
    Serial.print("Packet Length: ");
    Serial.println(strlen(txpacket));

    // **Send the packet over LoRa**
    Radio.Send((uint8_t*)txpacket, strlen(txpacket));

    lora_idle = false;  // Set to false to wait for TX complete callback
  }

  Radio.IrqProcess();  // Necessary to process LoRa events
}

void OnTxDone(void) {
  Serial.println("TX done......");
  lora_idle = true;
}

void OnTxTimeout(void) {
  Radio.Sleep();
  Serial.println("TX Timeout......");
  lora_idle = true;
}
