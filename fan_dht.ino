#include <IRremote.h> // Library for decoding signals from an IR remote
#include <DHT.h> // Library for reading from DHT temperature/humidity sensors

// Pin assignments
#define ENABLE 5 // L293D ENA pin (PWM for speed)
#define DIRA 3 // Motor direction input A pin
#define DIRB 4 // Motor direction input B pin

#define IR_RECEIVE_PIN 8 // IR receiver pin
#define DHTPIN 7 // DHT11 sensor pin
#define DHTTYPE DHT11 // Specify DHT sensor model

DHT dht(DHTPIN, DHTTYPE); // Creates a DHT11 object to start reading temperature (or humidity)

// Declare state variables
bool fanOn = false; // Tracks fan state (initialised to 0 due to it being a global variable (false))
bool manualOverride = false; // Blocks automatic temp activation if true (also intialised to 0)
unsigned long lastToggleTime = 0; // Debounce timer for IR remote

void setup() {
  Serial.begin(9600); // Starts serial communication at 9600 baud

  pinMode(ENABLE, OUTPUT); // Initialise the motor control pins as outputs
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  turnFanOff(); // Ensures the fan is off at the start

  dht.begin(); // Initialises the DHT sensor
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Starts IR receiver with optional LCD display for future implementation

  Serial.println("Fan Control (IR + DHT11) with Manual Override"); // Prints the message to the serial monitor
}

void loop() {
  // 1. Temperature Auto-On Logic (only if no manual override)
  float temp = dht.readTemperature(); // Reads the current temperature from the DHT11 sensor
  if (!isnan(temp)) { // Ensures the sensor reading is valid
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" °C"); // Prints the sensed temperature

    if (temp >= 25 && !fanOn && !manualOverride) { // If temperature is greater/equal to 25 degrees Celsius AND fan is currently off AND user hasn't manually overriden it
      fanOn = true; // Turns the fan on automatically
      turnFanOn(); 
      Serial.println("Auto ON: Temperature ≥ 25°C"); // Prints the message to user
    }
  } else {
    Serial.println("Failed to read temperature"); // Handles the failure case (no temp reading) for reliability
  }

  // 2. IR Remote Toggle Logic
  if (IrReceiver.decode()) { // Checks for an IR signal
    unsigned long code = IrReceiver.decodedIRData.command; // Decodes the IR signal
    IrReceiver.resume(); // Gets ready for the next IR signal

    if (code == 0x45 && millis() - lastToggleTime > 500) { // Checks if the POWER button on the remote (0x45) is pressed, and 500ms have passed
      fanOn = !fanOn; // Toggle the fan state

      if (fanOn) {
        turnFanOn();
        manualOverride = false; // Reset lockout if turning fan back on
        Serial.println("IR: Fan ON (Manual)");
      } else {
        turnFanOff();
        manualOverride = true; // Enable lockout when turning off manually
        Serial.println("IR: Fan OFF (Manual Override)");
      }

      lastToggleTime = millis(); // Update debounce timer
    }
  }

  delay(1000); // Wait 1 second before next sensor check
}

// Function that activates the motor in forward direction at full speed
void turnFanOn() {
  digitalWrite(DIRA, HIGH); // Sets the forward direction of motor
  digitalWrite(DIRB, LOW);
  analogWrite(ENABLE, 255); // Full power PWM
}

// Function that stops the motor completely
void turnFanOff() {
  digitalWrite(DIRA, LOW); // Disables both motor direction pins
  digitalWrite(DIRB, LOW);
  digitalWrite(ENABLE, LOW); // Fully stop PWM
}
