# Arduino Remote-Controlled Fan with Temperature Sensor
This project is an embedded system that controls a fan based on temperature input from a DHT11 sensor, with manual override via an IR remote. It demonstrates real-world applications of sensor intergation, control logic, and user interaction designed with a top-down modular approach and implemented bottom up.

## Features
- Automatic fan activation at >= 25 degrees Celsius
- Manual ON/OFF control using IR remote
- Lockout logic: fan stays off after manual override regardless of temperature

## System Architecture
Modular code structure with separate files for: 
- Testing for fan functionality (fan_test.ino)
- Integrating the IR Receiver module with the fan (fan2.ino)
- Adding the DHT11 sensor to the existing circuit (fan_dht.ino)

## Hardware Used
- Elegoo (Arduino) Uno R3
- 9V battery with snap-on connector clip
- Power supply module
- L293D motor driver
- Fan blade with 3-6V motor
- IR receiver + remote
- DHT11 temeprature sensor
- Breadboard and jumper wires

## How It Works
- The system reads the temperature every 1s.
- If the temperature is >= 25 degrees Celsius, the fan turns on.
- If the user turns the fan off via remote, it remains off even if the temperature is above the threshold.

## Future plans
- Integrate an LCD1602 module to display fan status and temperature instead of through the serial monitor.
- Combine the LCD with a 74HC595 shift register so that fewer I/O pins on the Arduino is used.
