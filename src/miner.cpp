#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>
#include <sstream>


struct SensorData {
    float temperature;
    float humidity;
    int light;
};

bool parseLine(const std::string& line, SensorData& data) {
    std::stringstream ss(line);
    std::string value;

    try {
        std::getline(ss, value, ',');
        data.temperature = std::stof(value);

        std::getline(ss, value, ',');
        data.humidity = std::stof(value);

        std::getline(ss, value, ',');
        data.light = std::stoi(value);
    }
    catch (...) {
        return false;  // parsing failed
    }

    return true;
}

int main() {
    const char* port = "/dev/ttyACM0";   // Port where the Arduino is connected
    int serial_port = open(port, O_RDONLY); // Open the serial port for reading only

    // Check if the port was opened successfully
    if(serial_port < 0) {
        std::cerr << "Error opening serial port\n";
        return 1;
    }

    // configure strcture that contains the settings for the serial port
    struct termios tty;
    memset(&tty, 0, sizeof tty);

    // Get the current configuration of the serial port
    if(tcgetattr(serial_port, &tty) != 0) {
        std::cerr << "Error from tcgetattr\n";
        return 1;
    }

    // Set the baud rate to 9600
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    tty.c_cflag |= (CLOCAL | CREAD); // local connection, enable reading
    tty.c_cflag &= ~CSIZE;           // Clear current character size mask
    tty.c_cflag |= CS8;              // 8 bits per byte
    tty.c_cflag &= ~PARENB;          // No parity bit
    tty.c_cflag &= ~CSTOPB;          // 1 stop bit
    tty.c_cflag &= ~CRTSCTS;         // Disable RTS/CTS hardware flow control, Arduino doesn't use it

    // Apply the configuration to the serial port
    tcsetattr(serial_port, TCSANOW, &tty);

    char buffer[256];
    std::string lineBuffer;

    while(true) {
        int n = read(serial_port, &buffer, sizeof(buffer) - 1);
        if(n > 0) {
            buffer[n] = '\0'; // make proper C-string
            lineBuffer += buffer; // Append the new data to the line buffer
            size_t pos;

            while ((pos = lineBuffer.find('\n')) != std::string::npos) { // pos is the position of the newline character
                std::string line = lineBuffer.substr(0, pos); // Extract the first line
                lineBuffer.erase(0, pos + 1); // Remove the processed line from the buffer

                SensorData data; // Create a SensorData struct to hold the parsed values
                if (parseLine(line, data)) {
                    std::cout << "Parsed Data - Temperature: " << data.temperature 
                              << " °C, Humidity: " << data.humidity 
                              << " %, Light: " << data.light << std::endl;
                }
                else {
                    std::cout << "Failed to parse line: " << line << std::endl;
                }
            }

        }
    }

    close(serial_port);
    return 0;
}
