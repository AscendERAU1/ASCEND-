// Library's
#include <stdio.h>
#include <math.h>


// Defining Known Variables
float a = 6378137;            // semi-major axis, meters
float f = 1 / 298.257223563;  // flattening
float b = (1 - f) * a;        // semi-minor axis
float S = 0;
float ForwardAzimuth = 0;
float ElevationAngle = 0;
float PI = 3.14159265358979323846;
float maxIter=200;
float tol=1*e^(-12);

void azmuth_Range() {

  //In void defines
  float phi_1, phi_2, L_1, U_1, U_2, L, L_2, L_1, lamda, sin_lamda, cos_lamda, cos_sigma, sin_sigma, sigma;
  float sin_alpha, cos2_alpha, cos2sigmam, C, lamda_prev, L;

  if (coordinate1 == coordinate2) {
    s = 0;
    ForwardAzimuth = 0;
    ElevationAngle = 0;
  }
  phi_1 = coordinate1[0] * PI / 180;
  L_1 = coordinate1[1] * PI / 180;

  phi_2 = coordinate1[0] * PI / 180;
  L_2 = coordinate1[1] * PI / 180;

  U_1 = atan((1 - self.f) * tan(phi_1));
  U_2 = atan((1 - self.f) * tan(phi_2));
 L = L_2 - L_1  // Change in longitude
lamda = L;
// Initialize values used in iterations
sin_U1 = sin(U_1);
cos_U1 = cos(U_1);
sin_U2 = sin(U_2);
cos_U2 = cos(U_2); // Cannot initialize lamda because it iterates

for (int i=0;i<maxIter;i++){
sin_lamda = sin(lamda)
cos_lamda = cos(lamda)  # Initialize lamda angles within iteration
sin_sigma = sqrt((cos_U2 * sin_lamda) ^ 2 + (cos_U1 * sin_U2 - sin_U1 * cos_U2 * cos_lamda) ^ 2)
cos_sigma = sin_U1 * sin_U2 + cos_U1 * cos_U2 * cos_lamda
sigma = atan2(sin_sigma, cos_sigma)
sin_alpha = (cos_U1 * cos_U2 * sin_lamda) / sin_sigma
cos2_alpha = 1 - sin_alpha ** 2
cos2sigmam = cos_sigma - (2 * sin_U1 * sin_U2 / cos2_alpha)
C = (self.f / 16) * cos2_alpha * (4 + self.f * (4 - 3 * cos2_alpha))
lamda_prev = lamda
lamda = L + (1 - C) * self.f * sin_alpha * (
sigma + C * sin_sigma * (cos2sigmam + C * cos_sigma * (-1 + 2 * cos2sigmam ** 2)))
test = abs(lamda_prev - lamda)
}
}

void main(){

}

