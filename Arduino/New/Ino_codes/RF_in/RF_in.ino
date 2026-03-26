#include <RH_ASK.h>
#include <SPI.h>

RH_ASK driver(2000, 12, 11, 10);  // speed, rxPin, txPin, pttPin

void setup() {
    Serial.begin(9600);
    if (!driver.init())
        Serial.println("RH_ASK init failed");
    else
        Serial.println("RH_ASK init OK");
}

void loop() {
    uint8_t buf[12];
    uint8_t buflen = sizeof(buf);
    if (driver.recv(buf, &buflen)) {
        Serial.print("Received: ");
        Serial.println((char*)buf);
    }
}