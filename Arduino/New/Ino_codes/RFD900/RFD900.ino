#define LIGHT 12
#include <SoftwareSerial.h>

SoftwareSerial rfd(-1, 9);

void setup() {
  pinMode(LIGHT, OUTPUT);
  delay(2000);
  Serial.begin(9600);
  Serial.println("RFD900 transmitter online");
  rfd.begin(9600);
}

void loop() {
  rfd.println("Hello from Arduino");
  digitalWrite(LIGHT, HIGH);
  digitalWrite(LIGHT, LOW);
  delay(2000);
}