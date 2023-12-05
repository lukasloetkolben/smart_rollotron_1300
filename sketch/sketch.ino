#include <SoftwareSerial.h>

// Pins Rx D5 (14) and Tx D6(12)
#define TX_PIN 12
#define RX_PIN 14
#define baudrate 9600

SoftwareSerial uart(RX_PIN, TX_PIN);

// Shutter Control Constants:
// 0x58: Represents the fully open position
// 0x54: Represents the fully closed position
// 0x51: Represents that the motor is currently running
// 0x50: Represents a partially open position

// Shutter Status Flags:
bool stoppedOpen = false;
bool stoppedClosed = false;
bool stopped = false;
bool running = false;

// Shutter Position Parameters:
long minPosition = 9999;   // Minimum recorded position of the shutter
long maxPosition = 0;      // Maximum recorded position of the shutter
long currentPosition = 0;  // Current position of the shutter

void setup() {
  Serial.begin(9600);
}

void uartStop() {
  uart.flush();
  uart.end();
  // pin floating
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, INPUT);
}

void down() {
  byte uartData[] = { 0x02, 0x02, 0x07, 0x00, 0x07, 0x03 };
  uart.begin(baudrate);
  uart.write(uartData, sizeof(uartData));
  uartStop();
}

void up() {
  byte uartData[] = { 0x02, 0x02, 0x04, 0x00, 0x04, 0x03 };
  uart.begin(baudrate);
  uart.write(uartData, sizeof(uartData));
  uartStop();
}

void readStatus() {
  const byte startMarker[] = { 0x02, 0x05 };
  const byte endMarker = 0x03;
  const int expectedResponseLength = 9;
  const unsigned long timeout = 200;

  byte uartData[] = { 0x02, 0x02, 0x41, 0x00, 0x041, 0x03 };
  uart.begin(baudrate);
  uart.write(uartData, sizeof(uartData));

  byte response[expectedResponseLength];
  int responseIndex = 0;
  unsigned long startTime = millis();

  while (millis() - startTime < timeout) {
    if (uart.available()) {
      byte receivedByte = uart.read();

      if (receivedByte != 0xFF) {
        response[responseIndex] = receivedByte;
        responseIndex++;

        if (responseIndex == sizeof(startMarker) && (memcmp(response, startMarker, sizeof(startMarker)) != 0)) {
          responseIndex = 0;
        }

        if (receivedByte == endMarker && responseIndex == expectedResponseLength) {

          // Postion
          currentPosition = response[6];

          if (currentPosition > maxPostion) {
            maxPostion = position;
          }
          if (currentPosition < minPostion) {
            minPostion = position;
          }

          // Open / Close
          stoppedClosed = false;
          stoppedOpen = false;
          stopped = false;
          running = false;
          switch (response[4]) {


            case 0x58:
              stoppedClosed = true;
              break;

            case 0x54:
              stoppedOpen = true;
              break;

            case 0x51:
              running = true;
              break;

            case 0x50:
              stopped = true;
              break;
          }

          uartStop();
          return;
        }
      }
    }
  }
  Serial.println("Timeout: No response received within 200ms");
  uartStop();
}


void loop() {
  readStatus();
  delay(500);

  if (maxPostion > minPostion) {
    float percentage = map(currentPosition, minPostion, maxPostion, 0, 100);
    Serial.print("Current position: ");
    Serial.print(percentage);
    Serial.println("%");
  }
}
