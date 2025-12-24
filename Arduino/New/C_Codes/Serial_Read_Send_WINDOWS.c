/*
COMPILE INSTRUCTIONS:

Open command prompt 

TYPE:

MinGW
gcc Serial_Read_Send_WINDOWS.c -o Serial_io.exe

MSVC
cl Serial_io.c

*/


#include <windows.h>
#include <stdio.h>
#include <string.h>

#define MAX_PORTS 16
#define MAX_LINE  512

/* Find available COM ports */
int find_ports(char ports[][16]) {
    int count = 0;

    for (int i = 1; i <= 32 && count < MAX_PORTS; i++) {
        char name[32];
        snprintf(name, sizeof(name), "\\\\.\\COM%d", i);

        HANDLE h = CreateFileA(
            name, GENERIC_READ | GENERIC_WRITE, 0,
            NULL, OPEN_EXISTING, 0, NULL
        );

        if (h != INVALID_HANDLE_VALUE) {
            snprintf(ports[count++], 16, "COM%d", i);
            CloseHandle(h);
        }
    }
    return count;
}

/* Configure serial: 115200 8N1 */
void setup_serial(HANDLE h) {
    DCB dcb = { .DCBlength = sizeof(dcb) };
    GetCommState(h, &dcb);

    dcb.BaudRate = CBR_115200;
    dcb.ByteSize = 8;
    dcb.Parity   = NOPARITY;
    dcb.StopBits = ONESTOPBIT;

    SetCommState(h, &dcb);

    COMMTIMEOUTS t = {50, 0, 50, 0, 50};
    SetCommTimeouts(h, &t);
}

/* Read one line */
int read_line(HANDLE h, char *buf, int max) {
    DWORD n;
    char c;
    int i = 0;

    while (i < max - 1) {
        ReadFile(h, &c, 1, &n, NULL);
        if (!n) continue;
        if (c == '\n') break;
        buf[i++] = c;
    }
    buf[i] = 0;
    return i;
}

int main(void) {
    char ports[MAX_PORTS][16];
    int count = find_ports(ports);

    if (!count) {
        printf("No COM ports found\n");
        return 1;
    }

    printf("Available ports:\n");
    for (int i = 0; i < count; i++)
        printf("[%d] %s\n", i, ports[i]);

    int in, out;
    printf("Select INPUT port: ");
    scanf("%d", &in);
    printf("Select OUTPUT port: ");
    scanf("%d", &out);

    char in_name[32], out_name[32];
    snprintf(in_name, 32, "\\\\.\\%s", ports[in]);
    snprintf(out_name,32, "\\\\.\\%s", ports[out]);

    HANDLE h_in  = CreateFileA(in_name,  GENERIC_READ,  0, NULL, OPEN_EXISTING, 0, NULL);
    HANDLE h_out = CreateFileA(out_name, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    setup_serial(h_in);
    setup_serial(h_out);

    printf("Forwarding '@ GPS_STAT' only...\n");

    char line[MAX_LINE];
    DWORD w;

    while (1) {
        int len = read_line(h_in, line, sizeof(line));
        if (len && !strncmp(line, "@ GPS_STAT", 10)) {
            WriteFile(h_out, line, len, &w, NULL);
            WriteFile(h_out, "\n", 1, &w, NULL);
            printf("Forwarded: %s\n", line);
        }
    }
}

