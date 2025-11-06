#include <string.h>

//Grabbing From serial
typedef struct {
  double altitude;
  double latitude;
  double longitude;
  double date;
  double time;
  bool valid;
} GPSData;

//Message through serial
char message[75];
char End_String[] = "CRC:";
String receivedString = "";


// Lengths Defines
#define MAX_FIELDS 50
#define MAX_FIELD_LENGTH 32
#define MAX_MESSAGE_LENGTH 256

// esp32 specific
// out states used for button
/*
int button1in= 32;
int button2in= 33;
int button1out= 25;
int button2out= 26;
int buttonstate=0;
int buttonstate2=0;
int i=0;
*/


// ============================
// Split message into fields
// ============================
int splitMessage(const char *message, char fields[MAX_FIELDS][MAX_FIELD_LENGTH]) {

  sscanf(message, "%*[^A]Alt %19s", fields[0]);
  sscanf(message,"%*[^A]lt %19s", fields[1]);
  sscanf(message,"%*[^A]ln %19s", fields[2]);
}

// ============================
// Extract GPS data
// ============================
GPSData extractGPS(const char *message) {
  char fields[MAX_FIELDS][MAX_FIELD_LENGTH];
  int fieldCount = splitMessage(message, fields);
  GPSData gps = { 0, 0, 0, 0, 0, false };

if (strncmp(message, "GPS_STAT", 8) == 0) {
  gps.altitude = atof(fields[0]);
  gps.latitude = atof(fields[1]);
  gps.longitude = atof(fields[2]);
  gps.date = atof(fields[3]);
  gps.time = atof(fields[4]);
  gps.valid = true;
}
  return gps;
}



void setup() {
  Serial.begin(115200);  // opens serial port, sets data rate to 115200 bps


  //button
  /*
  pinMode(button1in, INPUT_PULLUP);
  pinMode(button2in, INPUT_PULLUP);
  pinMode(button1out, OUTPUT);
  pinMode(button2out, OUTPUT);
  */
}

void loop() {


  // Example Message
  // @ GPS_STAT 202 0000 00 00 00:17:06.570 CRC_OK  TRK GPSTrk05169 Alt 000000 lt +00.00000 ln +00.00000 Vel +0000 +000 +0000 Fix 0 #  0  0  0  0 000_00_00 000_00_00 000_00_00 000_00_00 000_00_00 CRC: 5F95
  // Start message //Placeholder //Time     //CRC check not corrupt           //Latitude   //Longitude //Velocity           //GPS Fix type      //Additional raw satellite or time data (format placeholder)
  //Length of message                 //ID of Tracker  //Altitude                                                     //Sat tracking info                                       //Check for packet
  //IMPORTANT ONES:                                                                                 || The rest is unneeded
  // @ GPS_STAT                  :TIME                              ALT        LT           LN
  // Checking for serial will be used for featherweight
  /*if (Serial.available()) {

    char c = Serial.read();
    receivedString += c;

    // Check if the end of the string matches the terminator
    if (receivedString.endsWith("@")) {

      // Print the received string
      Serial.println("Received: " + receivedString);

      // Clear for next message
      receivedString = "";
    }

    if (receivedString.length() > 200) {
      Serial.println("Buffer overflow — clearing");
      receivedString = "";
    }
    delay(100);
  }
*/
  if (Serial.available()) {
    char c = Serial.read();
    receivedString += c;

    if (receivedString.endsWith("CRC:")) {
      Serial.println("Received: " + receivedString);
      receivedString = "";
    }

    if (receivedString.length() > 200) {
      Serial.println("Buffer overflow — clearing");
      receivedString = "";
    }
  }

  delay(1);  // Prevent watchdog resets


  if (message != "\n") {
    // Extracting GPS from the gotten serial message
    GPSData gps = extractGPS(message);


    //Prints data
    if (message[0] != '\0') {
      Serial.println(gps.latitude);
      Serial.println(gps.longitude);
      Serial.println(gps.altitude);
      Serial.println(gps.date);
      Serial.println(gps.time);
    }
    message[0] = '\0';
  }

  /*
  //Used for button state (NOT LEVER)
  buttonstate=digitalRead(button1in);
 if (buttonstate == LOW) {   // Button pressed
    digitalWrite(button1out, HIGH);  // Turn light ON
    Serial.println("Button Pressed - Light ON");
    i++;
    delay(500);
    if (i==2){
    digitalWrite(button1out, LOW);   // Turn light OFF
    i=0;
    }
  }
  buttonstate2=digitalRead(button2in);
if (buttonstate2 == LOW) {   // Button pressed
    digitalWrite(button2out, HIGH);  // Turn light ON
    Serial.println("Button Pressed - Light ON");
  } else {
    digitalWrite(button2out, LOW);   // Turn light OFF
    Serial.println("Button Released - Light OFF");
  }
  */
}
