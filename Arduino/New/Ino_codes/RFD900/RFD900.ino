#define LIGHT 12

void setup() {
  Serial.begin(9600); 
  pinMode(LIGHT, OUTPUT);

  delay(2000);
  Serial.println("RFD900 transmitter online");
}

void loop() {
  digitalWrite(LIGHT, HIGH);
  Serial.println("Hello over RFD900");
  digitalWrite(LIGHT, LOW);

  delay(1000);
}
