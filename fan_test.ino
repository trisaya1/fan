/*
  fan_test.ino

  Purpose: Standalone test script to validate basic motor functionality using
  the L293D motor driver and Arduino.

  This test ensures corrrect wiring, directional control (IN1/IN2), and PWM-based
  speed control (ENA) before integrating the motor into a larger embedded system.

  Design approach: follow a modular mindset - isolating and verifying hardware components
  individually before full system integration
*/

// Pin assignments for motor driver control
#define ENABLE 5    // PWM control pin connected to ENA on L293D (controls motor speed)
#define DIRA 3      // IN1 on L293D (motor direction input A)
#define DIRB 4      // IN2 on L293D (motor direction input B)

void setup() {
  Serial.begin(9600);           // Start serial communication for debugging and test feedback
  setupMotorPins();             // Set up all relevant motor control pins as outputs

  printHeader("Starting motor functionality test...");
  delay(1000);                  // Short pause to give time for serial monitor to open

  // Test 1: Run the motor at full speed in forward direction
  printHeader("Test 1: Full Speed Forward");
  runMotor(255, 3000);          // 255 = full PWM duty cycle, 3000ms = 3 seconds run time

  // Test 2: Gradually ramp up motor speed using PWM
  printHeader("Test 2: PWM Ramp Up");
  rampUpPWM();                  // Increments PWM value in steps to observe motor response

  // Test 3: Stop the motor completely
  printHeader("Test 3: Full Stop");
  stopMotor();                  // Sets all motor control pins to LOW
  delay(2000);                  // Pause for 2 seconds to confirm motor is stopped

  printHeader("Motor test complete.");
}

void loop() {
  // No continuous behavior needed — this is a one-time functional test
}

// ---------------------------------------------
// Function: setupMotorPins()
// Purpose: Configure motor-related pins as OUTPUT
// ---------------------------------------------
void setupMotorPins() {
  pinMode(ENABLE, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
}

// -------------------------------------------------
// Function: runMotor()
// Purpose: Runs motor at a specific speed and time
// @param speed: PWM duty cycle (0–255)
// @param duration: time in milliseconds
// -------------------------------------------------
void runMotor(int speed, int duration) {
  digitalWrite(DIRA, HIGH);       // Set direction: forward
  digitalWrite(DIRB, LOW);
  analogWrite(ENABLE, speed);     // Apply PWM speed
  delay(duration);                // Run motor for specified time
}

// ---------------------------------------------
// Function: stopMotor()
// Purpose: Stops the motor by setting all pins LOW
// ---------------------------------------------
void stopMotor() {
  analogWrite(ENABLE, 0);         // Stop sending PWM
  digitalWrite(DIRA, LOW);        // Disable direction inputs
  digitalWrite(DIRB, LOW);
}

// -------------------------------------------------
// Function: rampUpPWM()
// Purpose: Ramps up motor speed using increasing PWM
//          Useful for testing linear response of motor
// -------------------------------------------------
void rampUpPWM() {
  for (int speed = 0; speed <= 255; speed += 51) {
    analogWrite(ENABLE, speed);   // Apply current PWM level
    Serial.print("PWM: ");
    Serial.println(speed);        // Print current PWM value
    delay(1000);                  // Wait 1 second between steps
  }
}

// --------------------------------------------------------
// Function: printHeader()
// Purpose: Prints formatted headers to serial monitor
//          Makes debugging output more readable
// @param message: the message to display
// --------------------------------------------------------
void printHeader(String message) {
  Serial.println("--------------------------");
  Serial.println(message);
  Serial.println("--------------------------");
}
