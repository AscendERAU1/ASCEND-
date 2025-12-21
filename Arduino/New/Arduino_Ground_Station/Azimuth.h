#ifndef AZIMUTH_H
#define AZIMUTH_H

typedef struct {
  float S;
  float ForwardAzimuth;  // Gets Azimuth in DEG's
  float ElevationAngle;  // Gets Angle in DEG's
} AzimuthResult;

// Function prototype
AzimuthResult azmuth_Range(double coordinate1[2], double coordinate2[2], float altitude1, float altitude2);

#endif
