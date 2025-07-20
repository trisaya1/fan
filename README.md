# Arduino Remote-Controlled Fan with Temperature Sensor, IR Remote, and LCD diplay via shift register
This project is an embedded system that controls a DC fan based on temperature input from a DHT11 sensor, includes manual override via an IR remote, and displays real-time status using an LCD1602 screen connected through a 74HC595 shift register. It demonstrates real-world applications of control logi, sensor interaction, modular software design, and user interaction. 

## Features
- Automatic fan activation at >= 25 degrees Celsius
- Manual ON/OFF control via IR remote (NEC protocol)
- Lockout logic: fan remains OFF after manual override, even if temperature remains above threshold
- Real-time display of fan status and temperature on an LCD (only 3 Arduino pins used via shift register)

## System Architecture

The codebase is structured modularly and developed in logical stages, reflecting the top-down design and bottom-up implementation approach I took. Various diagnostic tests are included to ease debugging.

- 'fan_test.ino' - Testing L293D motor driver and fan direction/speed control.
- 'ir_test.ino' - Confirms IR remote signal decoding
- 'dht_test.ino' - Verifies standalone functionality of the DHT11 sensor
- 'lcd_test.ino' - Confirms LCD + 74HC595 communication

  
- 'fan2.ino' - Adds IR remote functionality for manual toggle to the existing fan
- 'fan_dht.ino' - Adds DHT11 temperature sensor and automatic fan logic to the existing fan + IR receiver
- 'fan_lcd.ino' - Final version: adds LCD1602 with shift register for low pin usage to the existing fan + IR receiver + DHT11 sensor 

## Hardware Used
- Elegoo Uno R3 (Arduino-compatible)
- 9V battery with snap-on connector clip
- Power supply module (for stable motor voltage)
- L293D motor driver IC
- 3-6V DC motor with attached fan blade
- IR receiver + remote (NEC protocol)
- DHT11 temeprature sensor (10k pull-up resistor between DATA and VCC for stability)
- Breadboard and jumper wires
- LCD1602 display
- 10k potentiometer (for LCD contrast)
- 74HC595 shift register
- 220 ohm resistors for A pin on LCD, and for stability between the arduino and the shift register

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
- Vertical Farming or Greenhouses: Fan or ventilation control systems that respond to ambient conditions with manual intervention as needed.

This kind of control logic is a foundation for embedded and IoT products where autonomy and human input must work together reliably.
