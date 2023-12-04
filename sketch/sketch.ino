#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

// Pins Rx D5 (14) and Tx D6(12)
#define RX_PIN 14
#define TX_PIN 12

SoftwareSerial uart(RX_PIN, TX_PIN);

void setup() {
  Serial.begin(115200);
  Serial.println("### Rollotron 1300 Controller ###");

}

void writeSerial(byte uartData[], size_t dataSize) {
  uart.begin(9600);
  uart.write(uartData, dataSize);
  uart.flush();
  delay(250);
  stopSerial();
}

void stopSerial() {
  uart.end();  // End the SoftwareSerial communication
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, INPUT);
}

void down() {
  byte uartData[] = { 0x02, 0x02, 0x07, 0x00, 0x07, 0x03 };
  writeSerial(uartData, sizeof(uartData));
}

void up() {
  byte uartData[] = { 0x02, 0x02, 0x04, 0x00, 0x04, 0x03 };
  writeSerial(uartData, sizeof(uartData));
}

void loop() {

}
