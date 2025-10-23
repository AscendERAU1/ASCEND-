int incomingByte = 0; // Start bit 

struct extra{
  int length[5];
  int year[4];
  int month;
  int day[2];
  int time[100];
}

// sd card UN NEEDED (hopefully)
#define SD_MOSI 23
#define SD_MISO 19
#define SD_SCLK 18
#define SD_CS 5


void setup() {
  Serial.begin(115200); // opens serial port, sets data rate to 115200 bps
  SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS); // sd card startup
}

void loop() {
  // send data only when you receive data:
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
}