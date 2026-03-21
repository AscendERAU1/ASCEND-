// This example shows how to send serial commands to two Tic
// Stepper Motor Controllers on the same serial bus.
//
// Each Tic's control mode must be set to "Serial/I2C/USB".  The
// serial device number of one Tic must be set to its default
// value of 14, and the serial device number of another Tic must
// be set to 15.
//
// The GND pin of the Arduino must be be connected to a GND pin
// on each Tic.  The TX pin of the Arduino must be connected to
// the RX pins of each Tic.  This example does not read data, so
// there is no need to connect the Tics' TX pins.
//
// See the comments and instructions in SerialSpeedControl.ino for
// more information.

#include <Tic.h>
#include <math.h>

// On boards with a hardware serial port available for use, use
// that port to communicate with the Tic. For other boards,
// create a SoftwareSerial object using pin 10 to receive (RX)
// and pin 11 to transmit (TX).
#ifdef SERIAL_PORT_HARDWARE_OPEN
#define ticSerial SERIAL_PORT_HARDWARE_OPEN
#else
#include <SoftwareSerial.h>
SoftwareSerial ticSerial(10, 11);
#endif

#define MAXSPEED 3000 // in steps per second
#define EARTH_RADIUS 6371000 // in meters

TicSerial yawStepper(ticSerial, 14);
TicSerial pitchStepper(ticSerial, 15);

//Pinion has 16 teeth, stator gear has 80 teeth, so each rotation of base requires 80/16 = 5 rotations of the motor, which is 5*200 = 1000 steps
//With planetary gear reduction of 1:100, each rotation of the output requires 100 rotations of the motor, which is 100*1000 = 100000 steps
//So to move 360 degrees, we need to move 100000 steps.

//Max speed is 3000 steps per second, to move 100000 steps, it will take 100000/3000 = 33.3 seconds, so we set the speed to 30000000 microsteps per 10000 seconds, which is 3000 steps per second.

void setup()
{
  Serial.begin(9600);
  ticSerial.begin(9600);
  delay(20);

  yawStepper.exitSafeStart();
  pitchStepper.exitSafeStart();
}

void resetCommandTimeout()
{
  yawStepper.resetCommandTimeout();
  pitchStepper.resetCommandTimeout();
}

void delayWhileResettingCommandTimeout(uint32_t ms)
{
  uint32_t start = millis();
  do
  {
    resetCommandTimeout();
  } while ((uint32_t)(millis() - start) <= ms);
}

double calculate_azimuth(double long1, double long2, double lat1, double lat2)
{
  //Inputs:
  // long1: longitude of the observer in radians
  // long2: longitude of the target in radians
  // lat1: latitude of the observer in radians
  // lat2: latitude of the target in radians
  //Output:
  // azimuth: the angle from the observer to the target in radians, measured clockwise from north
  //Purpose:
  // This function calculates the azimuth from the observer to the target.
  double dLong = long2 - long1;
  double x = sin(dLong) * cos(lat2);
  double y = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLong);
  double azimuth = atan2(x, y);
  if (azimuth < 0)
  {
    azimuth += 2 * PI;
  }
  //Return in radians, measured clockwise from north
  return azimuth;
}

double calculate_elevation(double long1, double long2, double lat1, double lat2, double h1, double h2)
{
  //Inputs:
  // long1: longitude of the observer in radians
  // long2: longitude of the target in radians
  // lat1: latitude of the observer in radians
  // lat2: latitude of the target in radians
  // h1: height of the observer above sea level in meters
  // h2: height of the target above sea level in meters
  //Output:
  // elevation: the angle from the observer to the target in radians, measured above the horizon
  //Purpose:
  // This function calculates the elevation from the observer to the target.
  double dlat = lat2 - lat1;
  double dlong = long2 - long1;
  double a = sin(dlat / 2) * sin(dlat / 2) + cos(lat1) * cos(lat2) * sin(dlong / 2) * sin(dlong / 2);
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));
  double d = EARTH_RADIUS * c;
  double r1 = EARTH_RADIUS + h1;
  double r2 = EARTH_RADIUS + h2;
  // Calculate the elevation angle (this is a simplified version and may need adjustment based on the specific application)
  double elevation = atan2(cos(c)-r1/r2, sin(c));
  return elevation;
}

void loop()
{
  
}