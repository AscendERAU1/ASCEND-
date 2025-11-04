#include <string.h>

//Grabbing From serial
typedef struct {
    double altitude;
    double latitude;
    double longitude;
    double dop;
    double sats;
    bool valid;
} GPSData;

//Message through serial
char message[75];
char End_String[]="CRC:";
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
    int count = 0;
    const char *start = message;
    const char *end;

    while ((end = strchr(start, ' ')) != NULL && count < MAX_FIELDS) {
        int len = end - start;
        if (len > 0) { // ignore empty segments
            if (len >= MAX_FIELD_LENGTH) len = MAX_FIELD_LENGTH - 1;
            strncpy(fields[count], start, len);
            fields[count][len] = '\0';
            count++;
        }
        start = end + 1;
    }

    // Take the last field (up to '*')
    if (*start != '\0' && count < MAX_FIELDS) {
        strncpy(fields[count], start, MAX_FIELD_LENGTH - 1);
        fields[count][MAX_FIELD_LENGTH - 1] = '\0';
        count++;

    }

    // Mimic the Python behavior: drop the last element
    if (count > 0) count--;

    return count;
}

// ============================
// Extract GPS data
// ============================
GPSData extractGPS(const char *message) {
    char fields[MAX_FIELDS][MAX_FIELD_LENGTH];
    int fieldCount = splitMessage(message, fields);

    GPSData gps = {0, 0, 0, 0, 0, false};
/*
    if (fieldCount > 19 && strcmp(fields[0], "@") == 0 && strcmp(fields[1], "GPS_STAT") == 0) {*/
        gps.altitude = atof(fields[11]);
        gps.latitude = atof(fields[13]);
        gps.longitude = atof(fields[15]);
        gps.dop = atof(fields[17]);
        gps.sats = atof(fields[19]);
        gps.valid = true;

    return gps;
}



void setup() {
  Serial.begin(115200); // opens serial port, sets data rate to 115200 bps


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
    if (Serial.available()) {
  /*
    String receivedString = Serial.readStringUntil("@"); // Read until CRC
    
    Serial.print("Received message: ");
    Serial.println(receivedString); //Prints incoming messages
    strncpy(message, receivedString.c_str(), sizeof(message));
    Serial.println(message); // Prints messages
    
  }
*/
///*
char c = Serial.read();
    receivedString += c;

    // Check if the end of the string matches the terminator
    if (receivedString.endsWith("@")) {
      // Remove the terminator from the result
      
      // Print the received string
      Serial.println("Received: " + receivedString);
      
      // Clear for next message
      receivedString = "";
    }
    }
//*/
  
  if(message!="\n"){
  // Extracting GPS from the gotten serial message
   GPSData gps = extractGPS(message);


  //Prints data
   if(message[0]!='\0'){
   Serial.println(gps.latitude);
    Serial.println(gps.longitude);
    Serial.println(gps.altitude);
    Serial.println(gps.dop);
    Serial.println(gps.sats);
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
