#include <stdio.h>
#include <math.h>

// Define constants
float a = 6378137.0f;             // semi-major axis (meters)
float f = 1.0f / 298.257223563f;  // flattening
float PI = 3.14159265358979323846f;
float maxIter = 200.0f;

// Define the struct
typedef struct {
    float S;
    float ForwardAzimuth;
    float ElevationAngle;
} AzimuthResult;

// Function declaration
AzimuthResult azmuth_Range(double coordinate1[2], double coordinate2[2], float altitude1, float altitude2) {
    AzimuthResult result;
    float b = (1.0f - f) * a;
    float tol = expf(-12.0f);

    // Local variables
    float phi_1, phi_2, U_1, U_2, L, L_2, L_1, lamda, sin_lamda, cos_lamda, cos_sigma, sin_sigma, sigma;
    float sin_alpha, cos2_alpha, cos2sigmam, C, lamda_prev, s;
    float sin_U1, sin_U2, cos_U1, cos_U2, test;

    // Check if points are identical
    if (coordinate1[0] == coordinate2[0] && coordinate1[1] == coordinate2[1]) {
        result.S = 0;
        result.ForwardAzimuth = 0;
        result.ElevationAngle = 0;
        return result;
    }

    // Convert degrees to radians
    phi_1 = coordinate1[0] * PI / 180;
    L_1 = coordinate1[1] * PI / 180;
    phi_2 = coordinate2[0] * PI / 180;
    L_2 = coordinate2[1] * PI / 180;

    // Reduced latitude
    U_1 = atan((1 - f) * tan(phi_1));
    U_2 = atan((1 - f) * tan(phi_2));

    L = L_2 - L_1;  // Change in longitude
    lamda = L;

    // Precompute trig terms
    sin_U1 = sin(U_1);
    cos_U1 = cos(U_1);
    sin_U2 = sin(U_2);
    cos_U2 = cos(U_2);

    // Iteration loop (Vincenty's formula)
    for (int i = 0; i < (int)maxIter; i++) {
        sin_lamda = sin(lamda);
        cos_lamda = cos(lamda);

        float sin_sigma_term1 = cos_U2 * sin_lamda;
        float sin_sigma_term2 = cos_U1 * sin_U2 - sin_U1 * cos_U2 * cos_lamda;
        sin_sigma = sqrtf(sin_sigma_term1 * sin_sigma_term1 + sin_sigma_term2 * sin_sigma_term2);

        cos_sigma = sin_U1 * sin_U2 + cos_U1 * cos_U2 * cos_lamda;
        sigma = atan2f(sin_sigma, cos_sigma);

        sin_alpha = (cos_U1 * cos_U2 * sin_lamda) / sin_sigma;
        cos2_alpha = 1 - sin_alpha * sin_alpha;
        cos2sigmam = cos_sigma - 2 * sin_U1 * sin_U2 / cos2_alpha;
        C = (f / 16) * cos2_alpha * (4 + f * (4 - 3 * cos2_alpha));

        lamda_prev = lamda;
        lamda = L + (1 - C) * f * sin_alpha * (
            sigma + C * sin_sigma * (cos2sigmam + C * cos_sigma * (-1 + 2 * cos2sigmam * cos2sigmam))
        );

        test = fabsf(lamda - lamda_prev);
        if (test <= tol) break;
    }

    // Compute surface distance (s)
    float u2 = cos2_alpha * (a * a - b * b) / (b * b);
    float A = 1 + u2 / 16384 * (4096 + u2 * (-768 + u2 * (320 - 175 * u2)));
    float B = u2 / 1024 * (256 + u2 * (-128 + u2 * (74 - 47 * u2)));
    float delta_sigma = B * sin_sigma * (cos2sigmam + B / 4 * (cos_sigma * (-1 + 2 * cos2sigmam * cos2sigmam)
                      - B / 6 * cos2sigmam * (-3 + 4 * sin_sigma * sin_sigma) * (-3 + 4 * cos2sigmam * cos2sigmam)));
    s = b * A * (sigma - delta_sigma);

    // Store results
    result.S = s;
    result.ForwardAzimuth = atan2f(cos_U2 * sin_lamda, cos_U1 * sin_U2 - sin_U1 * cos_U2 * cos_lamda) * 180 / PI;

    // Simple elevation calculation (optional placeholder)
    result.ElevationAngle = atan2f(altitude2 - altitude1, s) * 180 / PI;

    return result;
}

int main() {
    double coordinate1[2] = {34.0522, -118.2437};
    double coordinate2[2] = {34.06, -118.3};
    float altitude1 = 100.0;
    float altitude2 = 500.0;

    AzimuthResult output = azmuth_Range(coordinate1, coordinate2, altitude1, altitude2);

    printf("Distance (m): %f\n", output.S);
    printf("Forward Azimuth (deg): %f\n", output.ForwardAzimuth);
    printf("Elevation Angle (deg): %f\n", output.ElevationAngle);

    return 0;
}

