# UI For ASCEND

import tkinter as tk

# Initial values for X and Y coordinates
x_value = 0
y_value = 0

# Function placeholders (currently just print to console)
def set_ground_station_zero():
    print("Ground station dish set to zero.")
    global x_value, y_value
    x_value = 0
    y_value = 0
    update_display()

def translate_x(delta):

    # Pan Tilt Code Here?

    global x_value
    x_value += delta
    update_display()

def translate_y(delta):

    # Pan Tilt Code Here?

    global y_value
    y_value += delta
    update_display()

def activate_joystick():
    print("Joystick activated.")

def switch_to_automatic_tracking():
    print("Switching to automatic tracking.")

def terminate():
    print("Terminating Command")

def terminate_system():
    print("System terminated.")
    root.quit()  # Closes the application

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
        terminate_button = tk.Button(frame, text="Deactivate", command=terminate, font=("Times New Roman", 12), width=10)
        terminate_button.pack(side=tk.LEFT, padx=5)

# Add Set Ground Station Dish to Zero button (without terminate button)
frame_zero = tk.Frame(root)
frame_zero.pack(pady=10)
create_action_row_with_terminate(frame_zero, "Set Ground Station Dish to Zero", set_ground_station_zero, include_terminate=False)

# Add Activate Joystick button with terminate button
frame_joystick = tk.Frame(root)
frame_joystick.pack(pady=10)
create_action_row_with_terminate(frame_joystick, "Activate Joystick", activate_joystick)

# Add Switch to Automatic Tracking button with terminate button
frame_auto_tracking = tk.Frame(root)
frame_auto_tracking.pack(pady=10)
create_action_row_with_terminate(frame_auto_tracking, "Switch to Automatic Tracking", switch_to_automatic_tracking)

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
btn_terminate_system = tk.Button(root, text="Terminate System", command=terminate_system, font=("Times New Roman", 12), width=25)
btn_terminate_system.pack(pady=20)

# Start the Tkinter main loop
root.mainloop()
