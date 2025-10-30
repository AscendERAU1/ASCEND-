#include<string.h>

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
// Unchanging
int run=1;

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

    if (fieldCount > 19 && strcmp(fields[0], "@") == 0 && strcmp(fields[1], "GPS_STAT") == 0) {
        gps.altitude = atof(fields[11]);
        gps.latitude = atof(fields[13]);
        gps.longitude = atof(fields[15]);
        gps.dop = atof(fields[17]);
        gps.sats = atof(fields[19]);
        gps.valid = true;
    }

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

  // Only Defining once to not take more memory
  if (run == 1) {
    message[0] = '\0';  // start with an empty string
    run++;
  }

  // Example Message 
  // @ GPS_STAT X X X X X 123456 X X 0 0 500 34.12345 -117.12345 1.2 8 99 *

  // checking for serial will be used for featherweight
    if (Serial.available()) {
    String receivedString = Serial.readStringUntil('\n'); // Read until newline
    Serial.print("Received message: ");
    Serial.println(receivedString); //Prints incoming messages
    strncpy(message, receivedString.c_str(), sizeof(message) - 1);
    Serial.println(message); // Prints messages
    
  }
   Serial.println(message); // Prints messages



  
  if(message!="\n"){
  // Extracting GPS from the gotten serial message
   GPSData gps = extractGPS(message);
   message="\0";
   if(gps.latitude!=0){
   Serial.println(gps.latitude);
   }
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
