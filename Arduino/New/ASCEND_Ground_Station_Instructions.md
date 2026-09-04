# ASCEND! Ground Station — Startup Instructions

---

## 1. General Startup

1. Unbox the station and start the computer.
2. Plug in the Ground Station (GS) — make sure the **power switch is OFF**.
3. Plug the GS into the wall.
4. Connect the computer to the GS through the other port on the GS.
5. Grab the Featherweight GS and Featherweight GPS Tracker, and attach their antennas.

---

## 2. Computer Startup

1. Start the Arduino IDE and open `Arduino_Ground_Station.ino`.
2. Insert the current GPS latitude and longitude into the code, on the line:
   ```cpp
   float coordinate1[2] = { Lat, Long };  // Given Coords
   ```
3. With the code open, upload it to the Ground Station and confirm everything powers on in the Arduino GS.
4. > ⚠️ **IMPORTANT:** Turn off the Serial Monitor in the Arduino IDE.
5. Once the computer is connected to the GS, run `Serial_Read_Send_WINDOWS.c`.
6. Open **Device Manager** and check the COM ports.
7. Identify which COM ports correspond to the GS Featherweight and the GS Arduino.
8. In the program's input fields, enter the COM ports:
   - **Input** → GS Featherweight COM port
   - **Output** → GS Arduino COM port

---

## 3. Featherweight Startup

1. Turn on the Featherweight GS.
2. Turn on the Featherweight GPS Tracker — **only after the Ground Station is on.**
3. If not already connected, plug the Featherweight GS into the computer.

---

## 4. Ground Station Startup

1. Plug the joystick into the cables on the Ground Station.
   *Brown aligns to the ground pin — the rest fall into place.*
2. Make sure every switch is turned **off**.
3. Turn on the switch labeled **Joystick**.
4. Turn the switch near the power cable **on** — **only after the Arduino has been plugged in and uploaded to.**

---

## 5. Enable Auto Tracking

1. Confirm the Ground Station is in Joystick mode (the **Joystick** switch should be in the ON position).
2. Use the joystick to turn the Ground Station to face **north**.
3. Turn the **Joystick** switch **off**.
4. Once the Joystick switch is off, turn on the **Automatic Tracking** switch.

---

## 6. Troubleshooting

*(No entries yet — add known issues and fixes here.)*
