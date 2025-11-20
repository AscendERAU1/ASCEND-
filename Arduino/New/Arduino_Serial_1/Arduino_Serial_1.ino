#include <Arduino.h>

#define BUF_LEN 200
char msgBuf[BUF_LEN];
uint16_t msgIndex = 0;

bool inPacket = false;   // are we currently recording a packet?


typedef struct {
  double altitude;
  double latitude;
  double longitude;
  bool valid;
} GPSData;



GPSData extractGPS(const char *msg) {
  GPSData gps = {0,0,0,false};

  const char *altPtr = strstr(msg, "Alt ");
  const char *latPtr = strstr(msg, "lt ");
  const char *lonPtr = strstr(msg, "ln ");

  if (!altPtr || !latPtr || !lonPtr) {
    return gps;
  }

  char num[16];

  altPtr += 4;
  strncpy(num, altPtr, 6);
  num[6] = '\0';
  gps.altitude = atof(num);

  latPtr += 3;
  strncpy(num, latPtr, 10);
  num[10] = '\0';
  gps.latitude = atof(num);

  lonPtr += 3;
  strncpy(num, lonPtr, 10);
  num[10] = '\0';
  gps.longitude = atof(num);

  gps.valid = true;
  return gps;
}



void setup() {
  Serial.begin(115200);
  pinMode(12, OUTPUT);
}



void loop() {

  while (Serial.available()) {

    char c = Serial.read();

    // Detect start of packet
    if (!inPacket) {
      if (c == '@') {
        msgIndex = 0;
        msgBuf[msgIndex++] = c;
        inPacket = true;
      }
      continue;
    }

    // Store message safely
    if (msgIndex < BUF_LEN - 1) {
      msgBuf[msgIndex++] = c;
    } else {
      // overflow protection
      inPacket = false;
      msgIndex = 0;
      memset(msgBuf, 0, BUF_LEN);
      Serial.println("Buffer overflow prevented (cleared).");
      continue;
    }

    msgBuf[msgIndex] = '\0';

    // Detect end of message
    if (strstr(msgBuf, "CRC:")) {
      Serial.println("Received FULL packet:");
      Serial.println(msgBuf);

      GPSData gps = extractGPS(msgBuf);

      if (gps.valid) {
      // Flash LED
      digitalWrite(12, HIGH);
      delay(100);
      digitalWrite(12, LOW);
      } else {
        Serial.println("GPS parse failed.");
      }


      // reset state
      inPacket = false;
      msgIndex = 0;
      memset(msgBuf, 0, BUF_LEN);
    }

  }
}
