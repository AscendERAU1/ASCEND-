/*
COMPILE INSTRUCTIONS:

Open command prompt 

TYPE:
cd /(into directory it is in)

gcc Serial_Read_Send_LINUX.c -o Serial_io


./Serial_io

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#define MAX_PORTS 32
#define MAX_LINE 512

/* --------------------------------------------------
   Find available serial ports in /dev
   -------------------------------------------------- */
void find_serial_ports(char ports[][256], int *count) {
    DIR *dir;
    struct dirent *entry;

    *count = 0;
    dir = opendir("/dev");
    if (!dir) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL && *count < MAX_PORTS) {
        if (strncmp(entry->d_name, "ttyUSB", 6) == 0 ||
            strncmp(entry->d_name, "ttyACM", 6) == 0) {

snprintf(
    ports[*count],
    sizeof(ports[*count]),
    "/dev/%.250s",
    entry->d_name
);

            (*count)++;
        }
    }

    closedir(dir);
}

/* --------------------------------------------------
   Configure a serial port (115200 8N1, raw mode)
   -------------------------------------------------- */
int setup_serial(int fd) {
    struct termios tty;

    if (tcgetattr(fd, &tty) != 0) {
        perror("tcgetattr");
        return -1;
    }

    /* Baud rate */
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    /* 8N1 */
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag |= (CLOCAL | CREAD);

    /* Raw input/output */
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_oflag &= ~OPOST;

    /* Read behavior */
    tty.c_cc[VMIN]  = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("tcsetattr");
        return -1;
    }

    return 0;
}

/* --------------------------------------------------
   Read one full line from a serial port
   -------------------------------------------------- */
int read_line(int fd, char *line, int max_len) {
    int i = 0;
    char c;

    while (i < max_len - 1) {
        int n = read(fd, &c, 1);
        if (n <= 0)
            continue;

        if (c == '\n')
            break;

        line[i++] = c;
    }

    line[i] = '\0';
    return i;
}

/* --------------------------------------------------
   Main program
   -------------------------------------------------- */
int main(void) {
    char ports[MAX_PORTS][256];
    int port_count = 0;

    find_serial_ports(ports, &port_count);

    if (port_count == 0) {
        printf("No serial ports found.\n");
        return 1;
    }

    printf("\nAvailable serial ports:\n");
    for (int i = 0; i < port_count; i++) {
        printf("[%d] %s\n", i, ports[i]);
    }

    int in_idx, out_idx;
    printf("\nSelect INPUT port: ");
    scanf("%d", &in_idx);
    printf("Select OUTPUT port: ");
    scanf("%d", &out_idx);

    if (in_idx < 0 || in_idx >= port_count ||
        out_idx < 0 || out_idx >= port_count) {
        printf("Invalid selection.\n");
        return 1;
    }

    int fd_in  = open(ports[in_idx],  O_RDWR | O_NOCTTY | O_SYNC);
    int fd_out = open(ports[out_idx], O_RDWR | O_NOCTTY | O_SYNC);

    if (fd_in < 0 || fd_out < 0) {
        perror("open");
        return 1;
    }

    if (setup_serial(fd_in) < 0 || setup_serial(fd_out) < 0) {
        return 1;
    }

    printf("\nForwarding ONLY '@ GPS_STAT' messages...\n");

    char line[MAX_LINE];

    while (1) {
        int len = read_line(fd_in, line, sizeof(line));

        /* Only forward GPS_STAT lines */
        if (len > 0 && strncmp(line, "@ GPS_STAT", 10) == 0) {
            write(fd_out, line, len);
            write(fd_out, "\n", 1);

            printf("Forwarded: %s\n", line);
        }
    }

    close(fd_in);
    close(fd_out);
    return 0;
}

