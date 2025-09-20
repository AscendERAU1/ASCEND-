#include <Arduino.h>

#define CLOCKWISE 0
#define COUNTER_CLOCKWISE 1

const int IN1 = 7;
const int IN2 = 6;
const int IN3 = 5;
const int IN4 = 4;

const int BLUE = IN1;
const int RED = IN2;
const int GREEN = IN3;
const int BLACK = IN4;

void moveStepper(const int direction);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  moveStepper(CLOCKWISE);
  Serial.println("Running the loop!");
}

void moveStepper(const int direction){
  int delayTime = 10;
  if(direction == CLOCKWISE){
    //Step 1
    digitalWrite(BLACK, HIGH);
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
    delay(delayTime);
    //Step 2
    digitalWrite(BLACK, LOW);
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, LOW);
    delay(delayTime);
    //Step 3
    digitalWrite(BLACK, LOW);
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, HIGH);
    delay(delayTime);
    //Step 4
    digitalWrite(BLACK, HIGH);
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, HIGH);
    delay(delayTime);
  }else if(direction == COUNTER_CLOCKWISE){

  }else{
    Serial.println("Something is wrong!");
  }
}