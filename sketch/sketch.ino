#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>


// pins Rx D5 (14) and Tx D6(12)
SoftwareSerial uart(14, 12);  // RX, TX


void setup() {
  Serial.begin(115200);
  Serial.println("### Rollotron 1300 Controller ###");
}


void down() {
  byte uartData[] = { 0x02, 0x02, 0x07, 0x00, 0x07, 0x03 };
  Serial1.write(uartData, sizeof(uartData));
}

void up() {
  byte uartData[] = { 0x02, 0x02, 0x04, 0x00, 0x04, 0x03 };
  Serial1.write(uartData, sizeof(uartData));
}

void loop() {


}
