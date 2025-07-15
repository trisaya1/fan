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
  Serial.begin(9600); // Starts the serial monitor at 9600 baud

  // Configures the three motor control pins as outputs
  pinMode(ENABLE, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);

  Serial.println("Starting motor functionality test..."); // Prints the message to the serial monitor
  delay(1000); // Waits 1 second so the user can see it before the motor starts

  // Test 1: Run motor at full speed forward
  Serial.println("Test 1: Full Speed Forward");
  digitalWrite(DIRA, HIGH); // Sets IN1 pin HIGH
  digitalWrite(DIRB, LOW); // Sets IN2 pin LOW - motor therefore spins forward
  analogWrite(ENABLE, 255);  // Apply full speed (PWM 255 = 100%)
  delay(3000); // Run for 3 seconds

  // Test 2: Ramp up motor speed using PWM
  Serial.println("Test 2: PWM Ramp Up");
  for (int speed = 0; speed <= 255; speed += 51) {
    analogWrite(ENABLE, speed); // Gradually increase motor speed in steps of 51
    Serial.print("PWM: ");
    Serial.println(speed); // Print current PWM value
    delay(1000); // Wait 1 second between steps
  }

  // Test 3: Stop the motor 
  Serial.println("Test 3: Full Stop");
  analogWrite(ENABLE, 0); // Sets speed to 0
  digitalWrite(DIRA, LOW); // Stop both direction pins
  digitalWrite(DIRB, LOW);
  delay(2000); // Pause for 2 seconds to observe motor stop

  Serial.println("Motor test complete."); // Final message to confirm the test routine is complete
}

void loop() {
  // Empty loop function becausde this is a single-run diagnostic program, not a continuous control system
}
