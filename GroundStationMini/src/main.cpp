#include <Arduino.h>
#include <AccelStepper.h>

#define NUM_PINS 4
#define STEPS_PER_REV 100

using namespace std;

AccelStepper stepper1(NUM_PINS, 11, 10, 9, 8);
AccelStepper stepper2(NUM_PINS, 7, 6, 5, 4);
void testBackAndForth1();
void testBackAndForth2();
void testSpeedUpRampDown();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  stepper1.setMaxSpeed(500);
  stepper1.setAcceleration(100);
  stepper1.setMinPulseWidth(1);
  stepper2.setMaxSpeed(500);
  stepper2.setAcceleration(100);
  stepper2.setMinPulseWidth(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  stepper1.moveTo(5000);
  stepper2.moveTo(5000);
  while(stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0){
    stepper1.enableOutputs();
    stepper2.enableOutputs();
    stepper1.run();
    stepper2.run();
  }
  stepper1.disableOutputs();
  stepper2.disableOutputs();
  delay(2000);
  stepper1.moveTo(0);
  stepper2.moveTo(0);
  while(stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0){
    stepper1.enableOutputs();
    stepper2.enableOutputs();
    stepper1.run();
    stepper2.run();
  }
  stepper1.disableOutputs();
  stepper2.disableOutputs();
  delay(2000);
}

int position = 500;

void testSpeedUpRampDown(){
  for(int i = 0; i < 1000; i++){
    for(int j = 0; j < 300; j++ ){
      stepper1.setSpeed(pow(2, 0.01*i));
      stepper1.run();
    }
  }
  for(int i = 0; i < 30000; ++i){
    stepper1.setSpeed(pow(2, 30));
    stepper1.run();
  }
  for(int i = 1000; i > 0; i--){
    for(int j = 0; j < 300; j++ ){
      stepper1.setSpeed(pow(2, 0.01*i)); 
      stepper1.run();
    }
  }
  Serial.println("Cycled");
}

void testBackAndForth1(){
  Serial.println(String(stepper1.currentPosition()));

  stepper1.moveTo(position);
  if(stepper1.distanceToGo() == 0){
    Serial.println("Reached Target");
    stepper1.disableOutputs();
    delay(2000); // Wait for 2 seconds
    position = -position; // Reverse target position
  }else{
    stepper1.enableOutputs();
    stepper1.run();
  }
}

void testBackAndForth2(){
  Serial.println(String(stepper2.currentPosition()));

  stepper2.moveTo(position);
  if(stepper2.distanceToGo() == 0){
    Serial.println("Reached Target");
    stepper2.disableOutputs();
    delay(2000); // Wait for 2 seconds
    position = -position; // Reverse target position
  }else{
    stepper2.enableOutputs();
    stepper2.run();
  }
}