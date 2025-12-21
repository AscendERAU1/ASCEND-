#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>

#define MAX_PORTS 32

void list_serial_ports(char ports[][256], int *count) {
    struct dirent *entry;
    DIR *dp = opendir("/dev");

    *count = 0;
    if (dp == NULL) {
        perror("opendir /dev");
        return;
    }

    while ((entry = readdir(dp)) != NULL) {

        if (strncmp(entry->d_name, "ttyUSB", 6) == 0 ||
            strncmp(entry->d_name, "ttyACM", 6) == 0) {

            // Safe: prevents GCC warning
            snprintf(ports[*count], sizeof(ports[*count]),
                     "/dev/%.*s",
                     (int)(sizeof(ports[*count]) - 6),
                     entry->d_name);

            (*count)++;
            if (*count >= MAX_PORTS) break;
        }
    }
    closedir(dp);
}


int configure_port(int fd) {
    struct termios tty;
    if (tcgetattr(fd, &tty) != 0) {
        perror("tcgetattr");
        return -1;
    }

    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);

    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP |
                     INLCR | IGNCR | ICRNL);

    tty.c_oflag &= ~OPOST;

    tty.c_cc[VMIN]  = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("tcsetattr");
        return -1;
    }

    return 0;
}

int main() {
    char ports[MAX_PORTS][256];
    int port_count = 0;

    list_serial_ports(ports, &port_count);

    if (port_count == 0) {
        printf("No serial ports found.\n");
        return 1;
    }

    printf("\n=== Available Serial Ports ===\n");
    for (int i = 0; i < port_count; i++) {
        printf("[%d] %s\n", i, ports[i]);
    }

    int in_choice, out_choice;

    printf("\nSelect INPUT port number: ");
    scanf("%d", &in_choice);

    printf("Select OUTPUT port number: ");
    scanf("%d", &out_choice);

    if (in_choice < 0 || in_choice >= port_count ||
        out_choice < 0 || out_choice >= port_count) {
        printf("Invalid selection.\n");
        return 1;
    }

    const char* in_port = ports[in_choice];
    const char* out_port = ports[out_choice];

    printf("\nInput Port : %s\n", in_port);
    printf("Output Port: %s\n\n", out_port);

    int fd_in = open(in_port, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd_in < 0) {
        perror("open input port");
        return 1;
    }

    int fd_out = open(out_port, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd_out < 0) {
        perror("open output port");
        close(fd_in);
        return 1;
    }

    if (configure_port(fd_in) < 0 || configure_port(fd_out) < 0) {
        close(fd_in);
        close(fd_out);
        return 1;
    }

    printf("Forwarding data... (Ctrl+C to exit)\n");

    char buffer[256];
    int bytes;

    while (1) {
        bytes = read(fd_in, buffer, sizeof(buffer));
        if (bytes > 0) {
            write(fd_out, buffer, bytes);

            buffer[bytes] = '\0';
            printf("Forwarded: %s\n", buffer);
        }
    }

    close(fd_in);
    close(fd_out);
    return 0;
}

