#include <Arduino.h>

typedef struct {
  double altitude;
  double latitude;
  double longitude;
  double date;
  double time;
  bool valid;
} GPSData;

#define LIGHT 12

String receivedString = "";
char message[256];

GPSData extractGPS(const char *msg) {
  GPSData gps = {0, 0, 0, 0, 0, false};

  if (strncmp(msg, "@ GPS_STAT", 10) == 0) {
    // Parse altitude, lat, lon from message
    sscanf(msg, "%*[^A]Alt %lf lt %lf ln %lf", &gps.altitude, &gps.latitude, &gps.longitude);
    gps.valid = true;
  }

  return gps;
}

void setup() {
  Serial.begin(115200);
  pinMode(LIGHT, OUTPUT);
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();
    receivedString += c;

    if (receivedString.indexOf("@") != -1) {
      // Copy to C string buffer
      receivedString.toCharArray(message, sizeof(message));

      Serial.println("Received:");
      Serial.println(message);

      GPSData gps = extractGPS(message);
      if (gps.valid) {
        Serial.print("Altitude: "); Serial.println(gps.altitude);
        Serial.print("Latitude: "); Serial.println(gps.latitude);
        Serial.print("Longitude: "); Serial.println(gps.longitude);
      }

      receivedString = "";  // clear for next message
      digitalWrite(LIGHT, HIGH);
      delay(200);
      digitalWrite(LIGHT, LOW);
    }

    if (receivedString.length() > 200) {
      Serial.println("Buffer overflow — clearing");
      receivedString = "";
    }
  }
}
