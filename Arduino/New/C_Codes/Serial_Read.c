#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main() {
    int fd;
    const char *portname = "/dev/ttyUSB0"; // Adjust to your serial port

    fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        fprintf(stderr, "Error opening %s: %s\n", portname, strerror(errno));
        return 1;
    }

    struct termios tty;
    if (tcgetattr(fd, &tty) != 0) {
        fprintf(stderr, "Error from tcgetattr: %s\n", strerror(errno));
        close(fd);
        return 1;
    }

    cfsetospeed(&tty, B57600); // Set output baud rate
    cfsetispeed(&tty, B57600); // Set input baud rate

    tty.c_cflag |= (CLOCAL | CREAD); // Enable receiver, ignore modem control lines
    tty.c_cflag &= ~PARENB;          // No parity
    tty.c_cflag &= ~CSTOPB;          // 1 stop bit
    tty.c_cflag &= ~CSIZE;           // Clear data size bits
    tty.c_cflag |= CS8;              // 8 data bits

    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Raw input
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);         // Disable flow control
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable special handling of received bytes

    tty.c_oflag &= ~OPOST; // Raw output

    tty.c_cc[VMIN] = 1;  // Read at least 1 character
    tty.c_cc[VTIME] = 1; // 0.5 seconds timeout (5 * 0.1s)

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        fprintf(stderr, "Error from tcsetattr: %s\n", strerror(errno));
        close(fd);
        return 1;
    }

    char buffer[256];
    int bytes_read;

    while (1) {
        bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0'; // Null-terminate the received data
            printf("Received: %s\n", buffer);
        } else if (bytes_read < 0) {
            fprintf(stderr, "Error reading from serial port: %s\n", strerror(errno));
            break;
        }
        // Handle timeout (bytes_read == 0) if VMIN is set to 0
    }

    close(fd);
    return 0;
}
