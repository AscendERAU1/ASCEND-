#include <SPI.h>
#include <SD.h>
#include <FS.h>

int incomingByte = 0; // Start bit 

//Grabbing From serial
struct extra{
  int length[5];
  int year[4];
  int month;
  int day[2];
  int time[100];
};

// sd card UN NEEDED (hopefully) 
// Needed for testing to see what comes in
#define SD_MOSI 23
#define SD_MISO 19
#define SD_SCLK 18
#define SD_CS 5

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


void setup() {
  Serial.begin(115200); // opens serial port, sets data rate to 115200 bps
  SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS); // sd card startup

  //button
  /*
  pinMode(button1in, INPUT_PULLUP);
  pinMode(button2in, INPUT_PULLUP);
  pinMode(button1out, OUTPUT);
  pinMode(button2out, OUTPUT);
  */
}

void loop() {
  // send data only when you receive data:
  
  // checking for serial will be used for featherweight
  /*
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    //check for @
    if(incomingByte==64){

    }
    // say what you got:
    Serial.println("Start");    
    Serial.println(incomingByte);
    Serial.println(incomingByte, DEC);
    Serial.println("END");
  
  }
  */



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
