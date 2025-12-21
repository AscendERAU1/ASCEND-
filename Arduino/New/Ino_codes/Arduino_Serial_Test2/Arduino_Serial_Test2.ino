#include <Arduino.h>

#define LIGHT 12
#define BUFFER_SIZE 512

typedef struct {
  double altitude;
  double latitude;
  double longitude;
  bool valid;
} GPSData;

char buffer[BUFFER_SIZE];
size_t bufIndex = 0;

/* ---------- GPS PARSER ---------- */
GPSData extractGPS(const char *msg) {
  GPSData gps = {0, 0, 0, false};

  // Look ONLY for GPS_STAT messages
  if (strstr(msg, "@ GPS_STAT") != NULL) {

    // Parse Alt, lat, lon
    if (sscanf(msg,
               "%*[^A]Alt %lf lt %lf ln %lf",
               &gps.altitude,
               &gps.latitude,
               &gps.longitude) == 3) {
      gps.valid = true;
    }
  }

  return gps;
}

/* ---------- SETUP ---------- */
void setup() {
  Serial.begin(115200);
  pinMode(LIGHT, OUTPUT);

  Serial.println("GPS parser ready");
}

/* ---------- LOOP ---------- */
void loop() {
  while (Serial.available()) {
    char c = Serial.read();

    // Prevent buffer overflow
    if (bufIndex >= BUFFER_SIZE - 1) {
      bufIndex = 0;
    }

    buffer[bufIndex++] = c;
    buffer[bufIndex] = '\0';

    // End of message detected (CRC always ends the line)
    if (strstr(buffer, "CRC:") != NULL) {

      Serial.println("Received:");
      Serial.println(buffer);

      GPSData gps = extractGPS(buffer);

      if (gps.valid) {
        Serial.println("GPS DATA FOUND");
        Serial.print("Altitude: ");  Serial.println(gps.altitude);
        Serial.print("Latitude: ");  Serial.println(gps.latitude, 6);
        Serial.print("Longitude: "); Serial.println(gps.longitude, 6);

        digitalWrite(LIGHT, HIGH);
        delay(200);
        digitalWrite(LIGHT, LOW);
      }

      // Clear buffer for next message
      bufIndex = 0;
      buffer[0] = '\0';
    }
  }
}
