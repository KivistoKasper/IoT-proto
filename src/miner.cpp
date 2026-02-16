#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>

int main() {
    const char* port = "/dev/ttyACM0";   // Port where the Arduino is connected
    int serial_port = open(port, O_RDONLY);

    if(serial_port < 0) {
        std::cerr << "Error opening serial port\n";
        return 1;
    }

    struct termios tty;
    memset(&tty, 0, sizeof tty);

    if(tcgetattr(serial_port, &tty) != 0) {
        std::cerr << "Error from tcgetattr\n";
        return 1;
    }

    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    tcsetattr(serial_port, TCSANOW, &tty);

    char buffer[256];

    while(true) {
        int n = read(serial_port, &buffer, sizeof(buffer) - 1);
        if(n > 0) {
            buffer[n] = '\0';
            std::cout << buffer;
        }
    }

    close(serial_port);
    return 0;
}
