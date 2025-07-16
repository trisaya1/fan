#include <IRremote.h> // Allows the Arduino to receive and decode signals from an IR remote control

// Defines symbolic names for the motor control pins
#define ENABLE 5 // ENABLE pin controls the motor's speed via PWM
#define DIRA 3 // DIRA and DIRB pins sets the motor's direction using the L293D H-bridge
#define DIRB 4

// Specifies the digital pins used for receiving IR signals from the remote 
#define IR_RECEIVE_PIN 8

bool fanOn = false; // State flag for the fan - tracks whether the fan is currently ON or OFF
unsigned long lastToggleTime = 0; // Stores the last time the fan was toggled (used for debouncing)

void setup() { // Setup function only runs once as soon as Arduino boots up
  Serial.begin(9600); // Starts serial communication at 9600 baud

  pinMode(ENABLE, OUTPUT); // Sets the motor pins as outputs
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  turnFanOff(); // Ensures the fan is off at the start

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Starts IR receiver with optional onboard LED feedback (LED will be implemented later)
  Serial.println("IR Toggle Fan ON/OFF Ready"); // Prints the message to the serial monitor
}

void loop() { // Loop functions runs endlessly
  if (IrReceiver.decode()) { // Checks if a new IR signal has been received
    unsigned long command = IrReceiver.decodedIRData.command; // Stores the received IR command
    IrReceiver.resume(); // Prepare IR receiver for next signal

    // Responds only to the POWER button on the remote (0x45 in NEC protocol)
    if (command == 0x45 && millis() - lastToggleTime > 500) { // Implements software debounce: ignores repeated presses within 500ms to prevent rapid toggling
      fanOn = !fanOn; // Toggles the fan state

      // Calls the appropriate function turn the fan on or off then logs the action to the serial monitor  
      if (fanOn) {
        turnFanOn();
        Serial.println("Fan ON");
      } else {
        turnFanOff();
        Serial.println("Fan OFF");
      }

      lastToggleTime = millis();  // Updates the last toggle time for the next debounce check
    }
  }
}

// Function that activates the motor to full speed
void turnFanOn() {
  digitalWrite(DIRA, HIGH); // Set forware direction
  digitalWrite(DIRB, LOW);
  analogWrite(ENABLE, 255);  // Full speed via PWM
}

// Function that stops the motor
void turnFanOff() {
  digitalWrite(DIRA, LOW); // Disables direction pins
  digitalWrite(DIRB, LOW);
  digitalWrite(ENABLE, LOW);  // Fully disable motor by stopping PWM
}
