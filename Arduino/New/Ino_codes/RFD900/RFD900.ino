// this was the original


// this is a different one that could be used
#define LIGHT 12
#include <SoftwareSerial.h>

SoftwareSerial rfd(-1, 9); // RX disabled, TX only

void setup() {
  pinMode(LIGHT, OUTPUT);
  delay(2000);
  Serial.println("RFD900 transmitter online");
  rfd.begin(57600); // must match radio config
}

void loop() {
  rfd.println("Hello from Arduino");
  digitalWrite(LIGHT, HIGH);
  digitalWrite(LIGHT, LOW);
  delay(2000);
}
