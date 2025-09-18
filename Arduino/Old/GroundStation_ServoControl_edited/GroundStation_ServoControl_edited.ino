#include <Wire.h>
#include <Tic.h>
//unadded but not
// Define Tic controllers with their addresses
TicI2C tic1(14);  // Address for the first Tic (motor X)
TicI2C tic2(15);  // Address for the second Tic (motor Y)

// Constants for stepper motor parameters
const long stepsPerRevolution = 45701;  // Set this to your motor's steps per 360° rotation
int SetZeroValue = 0;
long speedX = 0;
long speedY = 0; 
int joyCenter = 512;
int deadzone = 200;
int lowerDeadzone = joyCenter - deadzone;
int upperDeadzone = joyCenter + deadzone;

// Joystick pins
const int joystickXPin = A0;
const int joystickYPin = A1;

// Joystick settings
const int centerThreshold = 100; // Deadzone threshold around center position
const long maxSpeed = 20000000;   // Max speed for the motor in steps per second (adjust as necessary)

void setup() {
  Wire.begin();         // Start I2C communication
  Serial.begin(115200);   // Initialize serial for input
  delay(1000);          // Small delay to ensure Serial connection is established
  Serial.println();

  // Initialize Tic controllers and exit safe start mode
  tic1.haltAndSetPosition(0);  // Set current position of tic1 to 0
  tic2.haltAndSetPosition(0);  // Set current position of tic2 to 0
  tic1.exitSafeStart();
  tic2.exitSafeStart();
}

// Sends a "Reset command timeout" command to both Tic controllers
void resetCommandTimeout() {
  tic1.resetCommandTimeout();
  tic2.resetCommandTimeout();
}

void SetZeroPosition() {
  tic1.haltAndSetPosition(0);  // Set current position of tic1 to 0
  tic2.haltAndSetPosition(0);  // Set current position of tic1 to 0
  tic1.exitSafeStart();
  tic2.exitSafeStart();
}

// Delays while resetting command timeout to avoid interruption
void delayWhileResettingCommandTimeout(uint32_t ms) {
  uint32_t start = millis();
  do {
    resetCommandTimeout();
  } while ((uint32_t)(millis() - start) <= ms);
}

void waitForPosition(TicI2C &ticController, int32_t targetPosition) {
  do {
    resetCommandTimeout();
  } while (ticController.getCurrentPosition() != targetPosition);
}



// Convert the angle to steps and set the target position for the specified motor
void setMotorPosition(char motor, int32_t angle) {
  int32_t steps = (angle * stepsPerRevolution) / 360;  // Convert angle to steps
// Note that the following sets a new zero point after each movemement for testing purposes. In future, use calibration message to set zero and move from there. 
  if (motor == 'X') {
    tic1.setTargetPosition(steps);  // Move tic1 (X motor) to the calculated position
    waitForPosition(tic1, steps);    
  } else if (motor == 'Y') {
    tic2.setTargetPosition(steps);  // Move tic1 (X motor) to the calculated position
    waitForPosition(tic2, steps);
  }
}

// Adjust motor speeds based on joystick input
void controlVelocityWithJoystick() {
  int joystickX = analogRead(joystickXPin);
  int joystickY = analogRead(joystickYPin);

  if ((joystickX >= lowerDeadzone) && (joystickX <= upperDeadzone)) {
    speedX = 0;
  } else if (joystickX < lowerDeadzone) {
    speedX = -maxSpeed;
  } else if (joystickX > upperDeadzone) {
    speedX = maxSpeed;
  }
  if ((joystickY >= lowerDeadzone) && (joystickY <= upperDeadzone)) {
    speedY = 0;
  } else if (joystickY < lowerDeadzone) {
    speedY = -maxSpeed;
  } else if (joystickY > upperDeadzone) {
    speedY = maxSpeed;
  }
  

  // Set motor speeds
  tic1.setTargetVelocity(speedX);
  tic2.setTargetVelocity(speedY);
/*
  Serial.print("Joystick X velocity: ");
  Serial.print(speedX);
  Serial.println(" steps/s");
  Serial.print("Joystick Y velocity: ");
  Serial.print(speedY);
  Serial.println(" steps/s");
*/
  // Ensure the Tic does not timeout
  delayWhileResettingCommandTimeout(100);
}

volatile bool newCommandReceived = false;  // Global flag for new command

void checkForNewCommand() {
  if (Serial.available() > 0) {
    newCommandReceived = true;  // Set the flag when new input is available
  }
}

void resetCommandFlag() {
  newCommandReceived = false;  // Reset the flag
}

void loop() {
  // Display prompt and wait for user input from the serial monitor
  Serial.println();

  // Wait for input from the serial monitor
  while (Serial.available() == 0) {
    delayWhileResettingCommandTimeout(10); // Reset timeout to avoid errors
    checkForNewCommand();  // Continuously check for new input
  }

  // Read input as a string
  String input = Serial.readStringUntil('\n');
  input.trim();  // Remove any extra whitespace or newline characters
  resetCommandFlag();  // Reset the flag after consuming the command

  // Handle tuple input for simultaneous motor movement
  if (input.startsWith("(") && input.endsWith(")")) {
    input = input.substring(1, input.length() - 1); // Remove parentheses
    int commaIndex = input.indexOf(',');           // Find the comma separator

    if (commaIndex != -1) {
      String xStr = input.substring(0, commaIndex);  // Extract X part
      xStr.trim();  // Trim whitespace from X part
      String yStr = input.substring(commaIndex + 1); // Extract Y part
      yStr.trim();  // Trim whitespace from Y part

      int32_t xAngle = xStr.toInt();  // Convert X string to integer
      int32_t yAngle = yStr.toInt();  // Convert Y string to integer

      if (SetZeroValue == 1) {
        int32_t xSteps = (xAngle * stepsPerRevolution) / 360; // Convert X angle to steps
        int32_t ySteps = (yAngle * stepsPerRevolution) / 360; // Convert Y angle to steps

        tic1.setTargetPosition(xSteps); // Move X motor
        tic2.setTargetPosition(ySteps); // Move Y motor

        while (tic1.getCurrentPosition() != xSteps || tic2.getCurrentPosition() != ySteps) {
          resetCommandTimeout();
          checkForNewCommand();  // Check for new input during motor movement
          if (newCommandReceived) return;  // Interrupt if new command is detected
        }
        /*
        Serial.print("Moved X motor to ");
        Serial.print(xAngle);
        Serial.println(" degrees");

        Serial.print("Moved Y motor to ");
        Serial.print(yAngle);
        Serial.println(" degrees");
        */
      } else {
        Serial.println("Zero position not set. Use 'S0' to set zero position first.");
      }
    } else {
      Serial.println("Invalid input format. Ensure input is in the format (X,Y).");
    }
  }
  // Handle 'J0' input for joystick control
  else if (input.equalsIgnoreCase("J0")) {

    while (true) {
      checkForNewCommand();  // Check for new input during joystick control
      if (newCommandReceived) return;  // Interrupt if new command is detected

      controlVelocityWithJoystick();  // Adjust motor speeds based on joystick input
    }
  }
  // Handle 'S0' input for setting zero position
  else if (input.equalsIgnoreCase("S0")) {
    SetZeroPosition();
    SetZeroValue = 1;
    Serial.println("Zero position set.");
  }
  // Handle single motor control input (e.g., X90 or Y-180)
  else if (input.length() > 1) {
    char motor = input.charAt(0);           // Get motor identifier ('X' or 'Y')
    int32_t angle = input.substring(1).toInt();  // Convert the rest to an integer for angle

    if (SetZeroValue == 1) {
      setMotorPosition(motor, angle);
    } else {
      setMotorPosition(motor, angle);
      SetZeroPosition();
    }
  }
  // Invalid input handling
  else {
    Serial.println("Invalid input. Please use the format (X,Y), J0, or S0.");
  }

  resetCommandTimeout();  // Reset command timeout to avoid Tic shutdown
  delay(50); // Delay for stability
}
