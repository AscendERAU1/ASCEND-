import serial
import time


class SerialComm:
    def __init__(self, port='COM3', baud_rate=115200, timeout=1):
        """Initialize the serial connection."""
        self.ser = serial.Serial(port, baud_rate, timeout=timeout)
        time.sleep(2)  # Wait for the serial connection to initialize

    def send_command_to_arduino(self, command):
        """Sends a command to the Arduino and reads the response."""
        self.ser.write((command + '\n').encode())
        time.sleep(0.1)  # Brief delay for stability
        self.read_arduino_response()

    def read_arduino_response(self):
        """Reads and prints responses from the Arduino."""
        while self.ser.in_waiting > 0:
            response = self.ser.readline().decode().strip()
            if response:
                print("Arduino:", response)

    def close_connection(self):
        """Closes the serial connection."""
        if self.ser.is_open:
            self.ser.close()
            print("Serial connection closed.")
