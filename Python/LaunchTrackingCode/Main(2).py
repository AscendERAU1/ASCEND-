import threading  # For running the tracking loop in a separate thread
from math import degrees
import time
from Serial_Comm import SerialComm
from Azimuth_Range import AzimuthRange
from Featherweight_Interface_Draft import FeatherweightParsing
from Featherweight_Interface_Draft import MessageReader
import tkinter as tk

# Setup
latcoordinate1 = input("Enter latitude of ground station in decimal form:\n")
longcoordinate1 = input("Enter longitude of ground station in decimal form:\n")
gs_alt = input("Enter altitude of ground station in feet:\n")
coordinate1 = (float(latcoordinate1), float(longcoordinate1))
gs_connection_test = 0
arduino_connection_test = 0
# Initial values for X and Y coordinates
x_value = 0
y_value = 0
terminate_active = False

while gs_connection_test != 1:
    try:
        parsing = FeatherweightParsing()  # Initialize parsing function
        AR = AzimuthRange()
        print("Ground station connected")
        gs_connection_test = 1
    except Exception as e:
        gs_connection_test = 0
        print(f"Error: {e}")
        print("Please connect ground station and ensure good connection to tracker")
        time.sleep(2)

while arduino_connection_test != 1:
    try:
        comm = SerialComm(port=f"COM{6}", baud_rate=115200, timeout=1)
        print("Tracking station connected")
        arduino_connection_test = 1
    except Exception as e:
        arduino_connection_test = 0
        print(f"Error: {e}")
        print("Please connect tracking station")
        time.sleep(2)


# Function definitions
def set_ground_station_zero():
    print("Ground station dish set to zero.")
    global x_value, y_value
    x_value = 0
    y_value = 0
    comm.send_command_to_arduino(f"S0")
    update_display()


def return_to_zero():
    global x_value, y_value
    print("Returning dish to zero")
    command = f"({0},{0})"
    comm.send_command_to_arduino(command)
    x_value = 0
    y_value = 0
    update_display()


def translate_x(delta):
    global x_value
    x_value += delta
    command = f"X{x_value}"
    comm.send_command_to_arduino(command)
    update_display()


def translate_y(delta):
    global y_value
    y_value += delta
    command = f"Y{y_value}"
    print(command)
    comm.send_command_to_arduino(command)
    update_display()


def activate_joystick():
    global terminate_active
    print("Joystick activated.")
    terminate_active = True  # Activate joystick loop
    comm.send_command_to_arduino(f"J0")

    while terminate_active:
        update_display()
        time.sleep(0.25)
    comm.send_command_to_arduino(f"stop")
    print("Joystick mode terminated")
    update_display()


def switch_to_automatic_tracking():
    print("Switching to automatic tracking.")
    global terminate_active, x_value, y_value
    terminate_active = True  # Activate tracking loop



    while terminate_active:
        update_display()
        message = parsing.message_reader.read_message()  # Get message from featherweight comm

        # storing the previous values so that it can be used later.
        previous_x = x_value
        previous_y = y_value

        if message:
            payload_alt, xcoord, ycoord, vx, vy = parsing.extract_gps_coords(message)  # Get GPS and velocities
            if payload_alt is not None:
                coordinate2 = (xcoord, ycoord)  # Assign GPS coordinate for payload
                if coordinate2[0] < -123.8571589028068 | | coordinate2[0] < -70.51451627738275:  # Lat (e-w)
                    break
                elif coordinate2[1] > 49.53851441184275 | | coordinate2[1] < 25.272085123304667:  # long (N-S)
                    break
                else

                AR.update(coordinate1, coordinate2, gs_alt, payload_alt)  # Update azimuth and range

                # Get values from azimuth and range code
                range_in_meters = AR.get_range()
                azimuth_in_radians = AR.get_azimuth()
                azimuth_in_degrees = degrees(azimuth_in_radians)
                elevation_angle = AR.get_elevation()
                elevation_angle_in_degrees = degrees(elevation_angle)

                # Debugging print statements
                print(f"Coordinate 1: {coordinate1}")
                print(f"Coordinate 2: {coordinate2}")
                print(f"Altitude: {payload_alt}")
                print(f"Range: {range_in_meters / 1000} kilometers")
                print(f"Forward Azimuth: {azimuth_in_degrees} degrees")
                print(f"Elevation: {elevation_angle_in_degrees}")

                # Update X and Y values
                x_value = -azimuth_in_degrees
                y_value = -elevation_angle_in_degrees
                if previous_y > 5:
                    previous_y = 5
                command = f"({previous_x},{previous_y})"
                comm.send_command_to_arduino(command)

                # Now call the CSV logging function
                parsing.parse_gps_to_csv(message)

        else:
            print("PACKET LOSS | RETAINING POSITION")
            command = f"({previous_x},{previous_y})"
            comm.send_command_to_arduino(command)

        time.sleep(0.25)

    print("Automatic tracking terminated.")  # Confirmation message




def terminate():
    """Stop automatic tracking or other active processes."""
    global terminate_active
    terminate_active = False  # Deactivate tracking
    print("Process terminated.")


def terminate_system():
    print("System terminated.")
    root.quit()  # Closes the application


def start_tracking_thread():
    tracking_thread = threading.Thread(target=switch_to_automatic_tracking)
    tracking_thread.daemon = True  # Allows the thread to exit when the program exits
    tracking_thread.start()


def start_joystick_thread():
    joystick_thread = threading.Thread(target=activate_joystick)
    joystick_thread.daemon = True  # Allows the thread to exit when the program exits
    joystick_thread.start()


# Update the displayed X and Y values
def update_display():
    x_label.config(text=f"X | {x_value}")
    y_label.config(text=f"Y | {y_value}")




# Set up the main Tkinter window
root = tk.Tk()
root.title("ASCEND UI")
root.geometry("450x450")  # Adjusted window size to fit all elements

# Add X and Y value labels to display the current values
x_label = tk.Label(root, text=f"X | {x_value}", font=("Times New Roman", 12))
x_label.pack(pady=5)

y_label = tk.Label(root, text=f"Y | {y_value}", font=("Times New Roman", 12))
y_label.pack(pady=5)


# Function to create a row with action button and terminate button
def create_action_row_with_terminate(frame, button_text, action_command, include_terminate=True):
    action_button = tk.Button(frame, text=button_text, command=action_command, font=("Times New Roman", 12), width=25)
    action_button.pack(side=tk.LEFT, padx=5)

    if include_terminate:
        terminate_button = tk.Button(frame, text="Deactivate", command=terminate, font=("Times New Roman", 12),
                                     width=10)
        terminate_button.pack(side=tk.LEFT, padx=5)


# Add Set Ground Station Dish to Zero button (without terminate button)
frame_zero = tk.Frame(root)
frame_zero.pack(pady=10)
create_action_row_with_terminate(frame_zero, "Set Ground Station Dish to Zero", set_ground_station_zero,
                                 include_terminate=False)

# Add Return to Zero button (without terminate button)
frame_zero = tk.Frame(root)
frame_zero.pack(pady=10)
create_action_row_with_terminate(frame_zero, "Return Dish to Zero", return_to_zero,
                                 include_terminate=False)

# Add Activate Joystick button with terminate button
frame_joystick = tk.Frame(root)
frame_joystick.pack(pady=10)
create_action_row_with_terminate(frame_joystick, "Activate Joystick", start_joystick_thread)

# Add Switch to Automatic Tracking button with terminate button
frame_auto_tracking = tk.Frame(root)
frame_auto_tracking.pack(pady=10)
create_action_row_with_terminate(frame_auto_tracking, "Switch to Automatic Tracking", start_tracking_thread)

# Add Translate X buttons (without terminate button)
frame_x = tk.Frame(root)
frame_x.pack(pady=10)
btn_minus_x = tk.Button(frame_x, text="-X", command=lambda: translate_x(-5), font=("Times New Roman", 12), width=10)
btn_minus_x.pack(side=tk.LEFT, padx=5)
btn_plus_x = tk.Button(frame_x, text="+X", command=lambda: translate_x(5), font=("Times New Roman", 12), width=10)
btn_plus_x.pack(side=tk.LEFT, padx=5)

# Add Translate Y buttons (without terminate button)
frame_y = tk.Frame(root)
frame_y.pack(pady=10)
btn_minus_y = tk.Button(frame_y, text="-Y", command=lambda: translate_y(-5), font=("Times New Roman", 12), width=10)
btn_minus_y.pack(side=tk.LEFT, padx=5)
btn_plus_y = tk.Button(frame_y, text="+Y", command=lambda: translate_y(5), font=("Times New Roman", 12), width=10)
btn_plus_y.pack(side=tk.LEFT, padx=5)

# Add Terminate System button at the bottom
btn_terminate_system = tk.Button(root, text="Terminate System", command=terminate_system, font=("Times New Roman", 12),
                                 width=25)
btn_terminate_system.pack(pady=20)

# Start the Tkinter main loop
root.mainloop()