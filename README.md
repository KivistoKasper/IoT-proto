# IoT-proto
COMP.CE.450-2026 Internet of Things 

## This project uses:
- Arduino Uno 
- Grove temperature&humidity sensor (High-Accuaracy & Mini)
- Grove light sensor

## Dependecies
- Seeed TH02 'https://github.com/Seeed-Studio/Grove_Temper_Humidity' 
- Grove light sensor library 'https://github.com/Seeed-Studio/Grove_LED_Bar/archive/master.zip'




## Run this in local linux
1. First things first
- Seeed HT02 sensor is a I2C digital sensor and thus needs to be connected to Arduino's "I2C" -port. 
- Arduino needs to be connected to a PC with a USB cable. External power source is not needed. 

2. Run the code 
```
g++ miner.cpp -o miner.out
./miner.out 
Mon Feb 16 19:00:42 2026
 - Temperature: 22.31 °C, Humidity: 31 %, Light: 31
Mon Feb 16 19:00:44 2026
 - Temperature: 22.19 °C, Humidity: 31.25 %, Light: 42
^C
```
