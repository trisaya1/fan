#include <IRremote.h>
#include <DHT.h>

// Pin definitions
#define ENABLE 5
#define DIRA 3
#define DIRB 4

#define IR_RECEIVE_PIN 8
#define DHTPIN 7
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

bool fanOn = false;
bool manualOverride = false; // true = block auto-on
unsigned long lastToggleTime = 0;

void setup() {
  Serial.begin(9600);

  pinMode(ENABLE, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  turnFanOff();

  dht.begin();
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  Serial.println("Fan Control (IR + DHT11) with Manual Override");
}

void loop() {
  // 1. Temperature Auto-On Logic (only if no manual override)
  float temp = dht.readTemperature();
  if (!isnan(temp)) {
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" °C");

    if (temp >= 20 && !fanOn && !manualOverride) {
      fanOn = true;
      turnFanOn();
      Serial.println("Auto ON: Temperature ≥ 20°C");
    }
  } else {
    Serial.println("Failed to read temperature");
  }

  // 2. IR Remote Toggle Logic
  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.command;
    IrReceiver.resume();

    if (code == 0x45 && millis() - lastToggleTime > 500) { // POWER button
      fanOn = !fanOn;

      if (fanOn) {
        turnFanOn();
        manualOverride = false; // reset lockout on manual ON
        Serial.println("IR: Fan ON (Manual)");
      } else {
        turnFanOff();
        manualOverride = true; // block temp auto-on
        Serial.println("IR: Fan OFF (Manual Override)");
      }

      lastToggleTime = millis();
    }
  }

  delay(1000); // Wait before next sensor check
}

// Turns fan ON
void turnFanOn() {
  digitalWrite(DIRA, HIGH);
  digitalWrite(DIRB, LOW);
  analogWrite(ENABLE, 255);
}

// Turns fan OFF
void turnFanOff() {
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
  digitalWrite(ENABLE, LOW);
}
