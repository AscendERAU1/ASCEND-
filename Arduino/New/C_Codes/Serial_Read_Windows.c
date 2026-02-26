#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hSerial;
    const char *portName = "\\\\.\\COM3";  // Change to your COM port

    hSerial = CreateFileA(portName,
                          GENERIC_READ | GENERIC_WRITE,
                          0,
                          0,
                          OPEN_EXISTING,
                          0,
                          0);

    if (hSerial == INVALID_HANDLE_VALUE) {
        printf("Error opening %s\n", portName);
        return 1;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        printf("Error getting serial state\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Set serial parameters
    dcbSerialParams.BaudRate = CBR_57600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity   = NOPARITY;
    dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        printf("Error setting serial parameters\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Set timeouts
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout         = 50;
    timeouts.ReadTotalTimeoutConstant    = 50;
    timeouts.ReadTotalTimeoutMultiplier  = 10;
    timeouts.WriteTotalTimeoutConstant   = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        printf("Error setting timeouts\n");
        CloseHandle(hSerial);
        return 1;
    }

    char buffer[256];
    DWORD bytesRead;

    while (1) {
        if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                printf("Received: %s\n", buffer);
            }
        } else {
            printf("Error reading from serial port\n");
            break;
        }
    }

    CloseHandle(hSerial);
    return 0;
}
