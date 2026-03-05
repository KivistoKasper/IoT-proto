# IoT-proto
COMP.CE.450-2026 Internet of Things 

## This project uses:
- Arduino Uno 
- Grove temperature&humidity sensor (High-Accuaracy & Mini)
- Grove light sensor

## Dependecies
### Arduino Uno 
- Seeed TH02 library 'https://github.com/Seeed-Studio/Grove_Temper_Humidity' 
- Arduino LCD liquidCrystalI2C library 'https://docs.arduino.cc/libraries/liquidcrystal-i2c/#Releases'

### Rasberry Pi 
- eclipse paho c++ client for MQTT 'https://github.com/eclipse-paho/paho.mqtt.cpp' 
- mosquitto and mosquitto-clients



## Running the code
### Flash the Arduino Uno

1. First things first connect the sensors
- Seeed HT02 sensor is a I2C digital sensor and thus needs to be connected to Arduino's "I2C" -port. 
- Arduino needs to be connected to a PC with a USB cable. External power source is not needed. 

2. Install dependecy library Seeed TH02 in the Arudino IDE

3. Flash the code in src/arduinoMain to the arduino with Arduino IDE

### Build and run the C++ code on rasberry Pi 

1. Install MQTT C++ client on the rasberry Pi. Follow the instructions on the GitHub page 

2. Build the code: `g++ miner.cpp -o miner.out -lpaho-mqttpp3 -lpaho-mqtt3as` 

3. Run the code `LD_LIBRARY_PATH=/usr/local/lib ./miner.out`


Capture from terminal: 
```
g++ miner.cpp -o miner.out
./miner.out 
Mon Feb 16 19:00:42 2026
 - Temperature: 22.31 °C, Humidity: 31 %, Light: 31
Mon Feb 16 19:00:44 2026
 - Temperature: 22.19 °C, Humidity: 31.25 %, Light: 42
^C
```
