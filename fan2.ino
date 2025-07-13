#include <IRremote.h>

// Motor control pins
#define ENABLE 5
#define DIRA 3
#define DIRB 4

// IR receiver pin
#define IR_RECEIVE_PIN 8

bool fanOn = false;                        // Fan state
unsigned long lastToggleTime = 0;         // For debouncing

void setup() {
  Serial.begin(9600);

  pinMode(ENABLE, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  turnFanOff();

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("IR Toggle Fan ON/OFF Ready");
}

void loop() {
  if (IrReceiver.decode()) {
    unsigned long command = IrReceiver.decodedIRData.command;
    IrReceiver.resume();

    // Only toggle on POWER button (code 0x45) with debounce
    if (command == 0x45 && millis() - lastToggleTime > 500) {
      fanOn = !fanOn;

      if (fanOn) {
        turnFanOn();
        Serial.println("Fan ON");
      } else {
        turnFanOff();
        Serial.println("Fan OFF");
      }

      lastToggleTime = millis();  // Update debounce timer
    }
  }
}

// Turn fan ON at full speed
void turnFanOn() {
  digitalWrite(DIRA, HIGH);
  digitalWrite(DIRB, LOW);
  analogWrite(ENABLE, 255);  // Full speed
}

// Turn fan OFF
void turnFanOff() {
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
  digitalWrite(ENABLE, LOW);  // Fully disable motor
}
