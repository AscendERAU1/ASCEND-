from math import atan, atan2, cos, radians, degrees, sin, sqrt, tan


class AzimuthRange:
    def __init__(self):
        # Define known variables
        self.a = 6378137  # semi-major axis, meters
        self.f = 1 / 298.257223563  # flattening
        self.b = (1 - self.f) * self.a  # semi-minor axis
        self.S = 0
        self.ForwardAzimuth = 0
        self.ElevationAngle = 0

    def update(self, coordinate1, coordinate2, altitude1, altitude2, maxIter=200, tol=1e-12):
        if coordinate1 == coordinate2:
            self.S = 0
            self.ForwardAzimuth = 0
            self.ElevationAngle = 0
            return
        phi_1, L_1 = radians(coordinate1[0]), radians(coordinate1[1])
        phi_2, L_2 = radians(coordinate2[0]), radians(coordinate2[1])
        U_1 = atan((1 - self.f) * tan(phi_1))
        U_2 = atan((1 - self.f) * tan(phi_2))
        L = L_2 - L_1  # Change in longitude

        lamda = L

        # Initialize values used in iterations 
        sin_U1 = sin(U_1)
        cos_U1 = cos(U_1)
        sin_U2 = sin(U_2)
        cos_U2 = cos(U_2)  # Cannot initialize lamda because it iterates

        # Iterations
        self.iters = 0
        for i in range(maxIter):
            self.iters += 1
            sin_lamda = sin(lamda)
            cos_lamda = cos(lamda)  # Initialize lamda angles within iteration
            sin_sigma = sqrt((cos_U2 * sin_lamda) ** 2 + (cos_U1 * sin_U2 - sin_U1 * cos_U2 * cos_lamda) ** 2)
            cos_sigma = sin_U1 * sin_U2 + cos_U1 * cos_U2 * cos_lamda
            sigma = atan2(sin_sigma, cos_sigma)
            sin_alpha = (cos_U1 * cos_U2 * sin_lamda) / sin_sigma
            cos2_alpha = 1 - sin_alpha ** 2
            cos2sigmam = cos_sigma - (2 * sin_U1 * sin_U2 / cos2_alpha)
            C = (self.f / 16) * cos2_alpha * (4 + self.f * (4 - 3 * cos2_alpha))
            lamda_prev = lamda
            lamda = L + (1 - C) * self.f * sin_alpha * (sigma + C * sin_sigma * (cos2sigmam + C * cos_sigma * (-1 + 2 * cos2sigmam ** 2)))

            test = abs(lamda_prev - lamda)
            if test <= tol:
                break

        u2 = cos2_alpha * ((self.a ** 2 - self.b ** 2) / self.b ** 2)
        A = 1 + (u2 / 16384) * (4096 + u2 * (-768 + u2 * (320 - 175 * u2)))
        B = (u2 / 1024) * (256 + u2 * (-128 + u2 * (74 - 47 * u2)))
        delta_sigma = B * sin_sigma * (
                cos2sigmam + 0.25 * B * (cos_sigma * (-1 + 2 * cos2sigmam ** 2)) - (1 / 6) * B * cos2sigmam * (
                -3 + 4 * sin_sigma ** 2) * (-3 + 4 * cos2sigmam ** 2))

        self.S = self.b * A * (sigma - delta_sigma)  # output range in meters
        # calculate forward azimuth
        self.ForwardAzimuth = atan2(cos_U2 * sin_lamda, cos_U1 * sin_U2 - sin_U1 * cos_U2 * cos_lamda)

        # calculate geocentric elevation angle
        h1, h2 = float(altitude1)*0.3048, float(altitude2)*0.3048
        d = sqrt((self.a + h1) ** 2 + (self.a + h2) ** 2 - 2 * (self.a + h1) * (self.a + h2) * cos(sigma))
        delta_altitude = h2 - h1
        self.ElevationAngle = atan2(delta_altitude, d)

    def get_range(self):
        return self.S

    def get_elevation(self):
        return self.ElevationAngle

    def get_azimuth(self):
        return self.ForwardAzimuth


# Example usage

"""coordinate1 = (35.65432881673358, -117.65783003449926)  # China Lake, CA
coordinate2 = (34.61672826636158, -112.4504148333023)   # ERAU Prescott
azimuth_range_instance = azimuth_range(coordinate1, coordinate2)
range_in_meters = azimuth_range_instance.get_range()
azimuth_in_radians = azimuth_range_instance.get_azimuth()
azimuth_in_degrees = degrees(azimuth_in_radians)
print(f"Coordinate 1: {coordinate1}")
print(f"Coordinate 1: {coordinate2}")
print(f"Range: {range_in_meters/1000} kilometers")
print(f"Forward Azimuth: {azimuth_in_degrees} degrees")"""
