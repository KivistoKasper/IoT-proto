# IoT-proto
COMP.CE.450-2026 Internet of Things 

## Overview of the Project

This project is a part of course work in COMP.CE.450 Internet of Things. The goal of this project was to implement and learn about IoT devices, IoT communication and IoT platforms. 

Implemented features in this project are the following:
- Use of 2 sensors, Temperature/humidity sensor and a light sensor.
- Basic connection with another device to send measured data.
- Use of IoT-platform to manage/analyze/control the output data (this can’t be used for the visualization feature).
- Visualization for the produced temperature and humidity in LCD screen connected to Arduino Uno.
- Actuation/decision making for switching home lights on and off.
- IoT geared protocol with MQTT to send data from Rasberry Pi to Home Assistant server.

Basics of how the devices work:
- Sensors gather temperature, humidity and light-level data.
- The Arduino reads the data from the sensors.
- The Arduino updates the data to show on a display connected to it.
- Raspberry Pi scrapes the data from the Arduino though USB connection.
- Raspberry Pi sends the data with MQTT to a PC running Home Assistant.
- Home Assistant controls a IoT smart home plug based on the data gathered from the sensors.


### Devices and Sensors of the Project
- Arduino Uno 
- Grove temperature&humidity sensor (High-Accuaracy & Mini)
- Grove light sensor
- Rasberry Pi 3B+
- Linux laptop with Home Assistant Docker service

## Dependecies

### Arduino Uno 
- Seeed TH02 library 'https://github.com/Seeed-Studio/Grove_Temper_Humidity' 
- Arduino LCD liquidCrystal I2C library by Marco Schwartz 'https://docs.arduino.cc/libraries/liquidcrystal-i2c/#Releases'

### Rasberry Pi 
- eclipse paho c++ client for MQTT 'https://github.com/eclipse-paho/paho.mqtt.cpp' 
- mosquitto and mosquitto-clients


## Running the code

### Flash the Arduino Uno

1. First things first connect the sensors
    - Grove temperature&humidity sensor is a I2C digital sensor and thus needs to be connected to Arduino's "I2C" -port. 
    - Grove light sensor is a analog sensor and needs to be connected to A0 port.
    - Arduino Uno needs to be connected to Rasberry Pi. External power source is not needed. 

2. Install dependecy librarys 'Seeed TH02' and 'LCD liquidCrystal I2C' in the Arudino IDE.

3. Flash the code in src/arduinoMain to the arduino with Arduino IDE.

### Build and run the C++ code on rasberry Pi 

1. Install Eclipse MQTT C++ client and mosquitto on the rasberry Pi. Follow the instructions on library's GitHub page.

2. Build the code and include the librarys: `g++ miner.cpp -o miner.out -lpaho-mqttpp3 -lpaho-mqtt3as` 

3. Run the code and include the librarys: `LD_LIBRARY_PATH=/usr/local/lib ./miner.out`

### Build Home Assistant docker image

1. Build the image: `docker compose up -d --build`

2. Wait for the building to finish and go through the setup process

3. Add the needed MQTT clients and controllable devices.
    - Temperature topic: `office/temp`
    - Humidity topic: `office/hum`
    - Light topic: `office/light` 
