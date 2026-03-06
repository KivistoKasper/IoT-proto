#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>
#include <sstream>
#include <chrono>
#include <ctime>
#include <mqtt/async_client.h>

// Server: 192.168.0.141
// Rasbi 192.168.0.69

const std::string SERVER_ADDRESS("tcp://192.168.0.69:1883");
const std::string CLIENT_ID("Rasbi");
const std::string TOPICLIGHT("office/light");
const std::string TOPICTEMP("office/temp");
const std::string TOPICHUM("office/hum");
const int QOS = 0;

struct SensorData {
    std::chrono::system_clock::time_point timestamp;
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

    data.timestamp = std::chrono::system_clock::now(); // Set the timestamp to the current time
    return true;
}

class PublisherCallback : public virtual mqtt::callback {
public:
    void connection_lost(const std::string& cause) override {
        std::cout << "Connection lost: " << cause << std::endl;
    }
    void delivery_complete(mqtt::delivery_token_ptr token) override {
        std::cout << "Message delivered" << std::endl;
    }
};

int main() {
    // set up MQTT client
    mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);
    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);

    // try connecting to the MQTT broker and set the callback for connection events and message delivery
    try {
        PublisherCallback callback;
        client.set_callback(callback);
        mqtt::token_ptr connToken = client.connect(connOpts);
        connToken->wait();
    } catch (const mqtt::exception& exc) {
        std::cerr << "Error connecting to MQTT broker: " << exc.what() << std::endl;
        return 1;
    }

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

    // Set the baud rate to 115200
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

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

    bool firstSkipped = false; // Flag to skip the first line if it contains headers

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
                    if (!firstSkipped) {
                        firstSkipped = true; // Skip the first line if it contains headers
                        continue;
                    }

                    std::string payload = "Temperature: " + std::to_string(data.temperature) + 
                                         " °C, Humidity: " + std::to_string(data.humidity) + 
                                          " %, Light: " + std::to_string(data.light);
                    
                    // Start publishing data with MQTT
                    mqtt::message_ptr pubmsg = mqtt::make_message(TOPICLIGHT, std::to_string(data.light));
                    client.publish(pubmsg)->wait(); // Publish the message to the MQTT broker
                    
                    pubmsg = mqtt::make_message(TOPICTEMP, std::to_string(data.temperature));
                    client.publish(pubmsg)->wait(); 
                    
                    pubmsg = mqtt::make_message(TOPICHUM, std::to_string(data.humidity));
                    client.publish(pubmsg)->wait(); 
                    
                    std::cout << "Message published!: " << payload << std::endl;
                }
            }
        }
    }

    close(serial_port);
    client.disconnect()->wait();
    return 0;
}
