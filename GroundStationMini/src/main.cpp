#include <Arduino.h>
#include <AccelStepper.h>

#define NUM_PINS 4
#define STEPS_PER_REV 100

using namespace std;

AccelStepper stepper1(NUM_PINS, 11, 10, 9, 8);
AccelStepper stepper2(NUM_PINS, 7, 6, 5, 4);
void testBackAndForth();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  stepper1.setMaxSpeed(1000);
  stepper1.setAcceleration(10000);
  stepper1.setMinPulseWidth(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 10; i++){
    for(int j = 0; j < 30000; j++ ){
      stepper1.setSpeed(pow(2, i));
      stepper1.run();
    }
  }
  for(int i = 10; i > 0; i--){
    for(int j = 0; j < 30000; j++ ){
      stepper1.setSpeed(pow(2, i)); 
      stepper1.run();
    }
  }
  Serial.println("Cycled");
  // testBackAndForth();
}

int position = 1000;

void testBackAndForth(){
  Serial.println(String(stepper1.currentPosition()));

  stepper1.moveTo(position);
  if(stepper1.distanceToGo() == 0){
    Serial.println("Reached Target");
    stepper1.disableOutputs();
    delay(10000); // Wait for 2 seconds
    position = -position; // Reverse target position
  }else{
    stepper1.enableOutputs();
    stepper1.run();
  }
}