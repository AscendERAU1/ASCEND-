#include <Arduino.h>
#include <AccelStepper.h>

#define CLOCKWISE 0
#define COUNTER_CLOCKWISE 1

using namespace std;

const int IN1 = 11;
const int IN2 = 10;
const int IN3 = 9;
const int IN4 = 8;

// const int BLUE = IN1;
// const int RED = IN2;
// const int GREEN = IN3;
// const int BLACK = IN4;

AccelStepper stepper1(4, IN1, IN2, IN3, IN4);
void testBackAndForth();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  stepper1.setMaxSpeed(1000);
  stepper1.setAcceleration(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  testBackAndForth();
}

int position = 200;

void testBackAndForth(){
  Serial.println(String(stepper1.currentPosition()));

  stepper1.moveTo(position);
  if(stepper1.distanceToGo() == 0){
    Serial.println("Reached Target");
    stepper1.disableOutputs();
    delay(5000); // Wait for 2 seconds
    position = -position; // Reverse target position
  }else{
    stepper1.enableOutputs();
    stepper1.run();
  }
}