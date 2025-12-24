// Library's
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


// Defining Known Variables
float a = 6378137.0f;             // semi-major axis, meters
float f = 1.0f / 298.257223563f;  // flattening (must use float division!)
float b;        // semi-minor axis
float S = 0.0f;
float ForwardAzimuth = 0.0f;
float ElevationAngle = 0.0f;
float PI = 3.14159265358979323846f;
float maxIter = 200.0f;
float tol;

void azmuth_Range(double coordinate1[2], double coordinate2[2], float altitude1, float altitude2) {

  //In void defines
  float phi_1, phi_2, U_1, U_2, L, L_2, L_1, lamda, sin_lamda, cos_lamda, cos_sigma, sin_sigma, sigma;
  float sin_alpha, cos2_alpha, cos2sigmam, C, lamda_prev, sin_sigma_1, sin_sigma_2, s;
  float sin_U1, sin_U2, cos_U1, cos_U2, test;
  tol = expf(-12.0f);
  b = (1.0f - f) * a; 

if (coordinate1[0] == coordinate2[0] && coordinate1[1] == coordinate2[1]){
    s = 0;
    ForwardAzimuth = 0;
    ElevationAngle = 0;
  }
  phi_1 = coordinate1[0] * PI / 180;
  L_1 = coordinate1[1] * PI / 180;

  phi_2 = coordinate2[0] * PI / 180;
  L_2 = coordinate2[1] * PI / 180;

  U_1 = atan((1 - f) * tan(phi_1));
  U_2 = atan((1 - f) * tan(phi_2));
  L = L_2 - L_1;  // Change in longitude
  lamda = L;
    // Initialize values used in iterations
    sin_U1 = sin(U_1);
    cos_U1 = cos(U_1);
    sin_U2 = sin(U_2);
    cos_U2 = cos(U_2); // Cannot initialize lamda because it iterates

for (int i=0;i<maxIter;i++){
sin_lamda = sin(lamda);
cos_lamda = cos(lamda);  // Initialize lamda angles within iteration

 sin_sigma_1 = (cos_U2 * sin_lamda) * (cos_U2 * sin_lamda);
 sin_sigma_2 = (cos_U1 * sin_U2 - sin_U1 * cos_U2 * cos_lamda) * (cos_U1 * sin_U2 - sin_U1 * cos_U2 * cos_lamda);

sin_sigma = sqrtf(sin_sigma_1 + sin_sigma_2);

cos_sigma = sin_U1 * sin_U2 + cos_U1 * cos_U2 * cos_lamda;
sigma = atan2(sin_sigma, cos_sigma);
sin_alpha = (cos_U1 * cos_U2 * sin_lamda) / sin_sigma;
cos2_alpha = powf((1 - sin_alpha), 2);
cos2sigmam = cos_sigma - (2 * sin_U1 * sin_U2 / cos2_alpha);
C = (f / 16) * cos2_alpha * (4 + f * (4 - 3 * cos2_alpha));
lamda_prev = lamda;
lamda = L + (1 - C) * f * sin_alpha * (sigma + C * sin_sigma * (cos2sigmam + C * cos_sigma * (-1 + 2 * (cos2sigmam*cos2sigmam))));

test = abs(lamda_prev - lamda);

    if (test <= tol){
    break;
    }

    }
    return;
}

void main(){

    double coordinate1[2] = {34.0522, -118.2437};  // Example: Los Angeles
    double coordinate2[2] = {40.7128, -74.0060};   // Example: New York
    float altitude1 = 100.0;  // meters
    float altitude2 = 500.0;  // meters
    double coord1[2], coord2[2];
    float alt1, alt2;
    coord1[],coord2[],alt1,alt2=azmuth_Range(coordinate1, coordinate2, altitude1, altitude2);
    printf(" Coorinate1: %f \n",coordinate1);
    printf(" Coorinate2: %f \n",coordinate2);
    printf(" Alt1: %f \n",altitude1);
    printf(" Alt2: %f \n",altitude2);
    

    return;
}

