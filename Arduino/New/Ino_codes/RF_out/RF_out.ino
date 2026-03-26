#include <RH_ASK.h>
#include <SPI.h>

RH_ASK driver(2000, 12, 11, 10);

void setup() {
    Serial.begin(9600);
    if (!driver.init())
        Serial.println("init FAILED");
    else
        Serial.println("init OK");
}

void loop() {
    const char *msg = "Hello";
    if (driver.send((uint8_t *)msg, strlen(msg)))
        Serial.println("Send OK");
    else
        Serial.println("Send FAILED");
    driver.waitPacketSent();
    delay(1000);
}