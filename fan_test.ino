/*
  fan_test.ino
  Standalone motor (fan) functionality test
  Purpose: Validate motor wiring, L293D driver, and direction control

  System Mindset: Isolate hardware blocks before full integration
*/

#define ENABLE 5    // L293D ENA pin (used for PWM control of motor speed)
#define DIRA 3      // L293D IN1 pin (controls motor direction A)
#define DIRB 4      // L293D IN2 pin (controls motor direction B)

// Setup function runs once when the Arduino starts up
void setup() { 
  Serial.begin(9600);
  pinMode(ENABLE, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);

  Serial.println("Starting motor functionality test...");
  delay(1000);

  // 1. Run motor at full speed
  Serial.println("Test 1: Full Speed Forward");
  digitalWrite(DIRA, HIGH);
  digitalWrite(DIRB, LOW);
  analogWrite(ENABLE, 255);  // Full power
  delay(3000);

  // 2. Test PWM ramp
  Serial.println("Test 2: PWM Ramp Up");
  for (int speed = 0; speed <= 255; speed += 51) {
    analogWrite(ENABLE, speed);
    Serial.print("PWM: ");
    Serial.println(speed);
    delay(1000);
  }

  // 3. Stop the motor
  Serial.println("Test 3: Full Stop");
  analogWrite(ENABLE, 0);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
  delay(2000);

  Serial.println("Motor test complete.");
}

void loop() {
  // Do nothing — one-time test
}
