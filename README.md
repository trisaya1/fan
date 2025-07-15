# Arduino Remote-Controlled Fan with Temperature Sensor
This project is a basic embedded system that controls a fan based on temperature input from a DHT11 sensor, with manual override via an IR remote. It demonstrates real-world applications of sensor intergation, control logic, and user interaction designed with a top-down modular approach and implemented bottom up.

## Features
- Automatic fan activation at >= 25 degrees Celsius
- Manual ON/OFF control using IR remote
- Lockout logic: once turned off manually, the fan stays off regardless of temperature

## System Architecture

The code is modular and was developed in stages: 

- 'fan_test.ino' - Testing fan activation using various digital signals
- 'fan2.ino' - IR receiver integration for manual fan control
- 'fan_dht.ino' - Final version: adds DHT11 temperature sensor to the existing IR + fan control system

## Hardware Used
- Elegoo Uno R3 (Arduino-compatible)
- 9V battery with snap-on connector clip
- Power supply module (for stable motor voltage)
- L293D motor driver
- Fan blade with 3-6V motor
- IR receiver + remote (NEC protocol)
- DHT11 temeprature sensor
- Breadboard and jumper wires

## How It Works
- The system continuously reads the temperaure once per second.
- If the temperature is 25 degrees Celsius or higher, the fan turns on automatically.
- If the user presses the IR remote's "Power" button to turn the fan off, the fan remains off even if the temperature stays above the threshold.
- This manual override is controlled via logic lockout to prevent automatic reactivation.

## Future Improvements
- Add an LCD1602 dmodule to show the current fan state and temperature instead of through the serial monitor.
- Combine the LCD with a 74HC595 shift register so that fewer I/O pins on the Arduino is used, improbing pin efficiency and scalability.

## Real-World Use Cases

This project demonstrates logic that can be extedned to several real-world applications:

- Environmental Chambers: Automatically regulate airflow or cooling based on internal temperature, while allowing manual override for safety or testing procedures.
- Smart Home Devices: Smart fans or HVAC systems that combine automatic control with user input for comfort and energy savings.
- Server Cooling Systems: Fan systems that activate based on heat levels but allow manual control during maintenance.
- Verical Farming or Greenhouses: Fan or ventilation control systems that respond to ambient conditions with manual intervention as needed.

This kind of control logic is a foundation for embedded and IoT products where autonomy and human input must work together reliably.
