#include <IRremote.h>
#include <DHT.h>
#include <LiquidCrystal_74HC595.h>

// ------------------ Pin Definitions ------------------
#define ENABLE 5         // Motor EN (PWM)
#define DIRA 3           // Motor IN1
#define DIRB 4           // Motor IN2
#define IR_RECEIVE_PIN 8 // IR receiver pin
#define DHTPIN 7         // DHT11 sensor pin

// ------------------ LCD Shift Register Pins ------------------
#define LCD_DATA  A1     // 74HC595 DS (Serial Data)
#define LCD_CLOCK A2     // 74HC595 SHCP (Shift Clock)
#define LCD_LATCH A3     // 74HC595 STCP (Storage Clock / Latch)

// ------------------ Libraries & State ------------------
DHT dht(DHTPIN, DHT11);
LiquidCrystal_74HC595 lcd(LCD_DATA, LCD_CLOCK, LCD_LATCH, 1, 3, 4, 5, 6, 7);

bool fanOn = false;
bool manualOverride = false;
unsigned long lastToggleTime = 0;
unsigned long lastTempRead = 0;
unsigned long lastLCDUpdate = 0;
float currentTemp = 0.0;
String fanStatus = "OFF";

// For LCD refresh optimization and stability
float lastDisplayedTemp = -999;
String lastDisplayedStatus = "";
bool lcdNeedsRefresh = true;

void setup() {
  Serial.begin(9600);
  
  // Motor setup
  pinMode(ENABLE, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  turnFanOff();
  
  // Sensors and peripherals
  dht.begin();
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  
  // LCD Initialization with proper timing
  delay(200);                   // Allow hardware to stabilize
  lcd.begin(16, 2);             // Initialize LCD (16 columns, 2 rows)
  delay(100);                   // Extra delay for shift register sync
  
  // Clear LCD and show startup message
  lcd.clear();
  delay(50);
  lcd.setCursor(0, 0);
  delay(10);
  lcd.print("Fan System Ready");
  delay(1500);
  
  // Clear and prepare for normal operation
  lcd.clear();
  delay(50);
  lcdNeedsRefresh = true;
  
  Serial.println("System Ready: IR + DHT11 + LCD");
}

void loop() {
  // 1. Read temperature every 2 seconds
  if (millis() - lastTempRead > 2000) {
    float temp = dht.readTemperature();
    
    if (!isnan(temp)) {
      currentTemp = temp;
      Serial.print("Temperature: ");
      Serial.print(currentTemp);
      Serial.println(" C");
      
      // Auto fan ON if not overridden
      if (currentTemp >= 25 && !fanOn && !manualOverride) {
        fanOn = true;
        fanStatus = "ON";
        turnFanOn();
        lcdNeedsRefresh = true;
        Serial.println("Auto ON: Temp ≥ 25°C");
      }
    } else {
      Serial.println("Temp read failed");
    }
    lastTempRead = millis();
    lcdNeedsRefresh = true;
  }
  
  // 2. IR Remote Control (POWER button toggle)
  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.command;
    IrReceiver.resume();
    
    if (code == 0x45 && millis() - lastToggleTime > 500) {
      fanOn = !fanOn;
      
      if (fanOn) {
        fanStatus = "ON";
        manualOverride = false;
        turnFanOn();
        Serial.println("IR: Fan ON (Manual)");
      } else {
        fanStatus = "OVERRIDE";
        manualOverride = true;
        turnFanOff();
        Serial.println("IR: Fan OFF (Override)");
      }
      lastToggleTime = millis();
      lcdNeedsRefresh = true;
    }
  }
  
  // 3. LCD Display Update (controlled timing and refresh)
  if (lcdNeedsRefresh && millis() - lastLCDUpdate > 300) {
    updateLCDDisplay();
    lcdNeedsRefresh = false;
    lastLCDUpdate = millis();
  }
  
  delay(50);  // Reduced delay for better responsiveness
}

// Dedicated LCD update function for better control
void updateLCDDisplay() {
  // Check if we actually need to update
  if (abs(currentTemp - lastDisplayedTemp) < 0.1 && fanStatus == lastDisplayedStatus) {
    return;
  }
  
  // Disable interrupts during LCD update for stability
  noInterrupts();
  
  // Update temperature line
  lcd.setCursor(0, 0);
  delay(10);
  
  // Create complete line strings to avoid partial updates
  String tempLine = "Temp: ";
  if (!isnan(currentTemp)) {
    tempLine += String(currentTemp, 1);
    tempLine += (char)223;  // Degree symbol
    tempLine += "C";
  } else {
    tempLine += "Error";
  }
  
  // Pad line to 16 characters to clear any remnants
  while (tempLine.length() < 16) {
    tempLine += " ";
  }
  
  lcd.print(tempLine);
  delay(10);
  
  // Update fan status line
  lcd.setCursor(0, 1);
  delay(10);
  
  String fanLine = "Fan: " + fanStatus;
  // Pad line to 16 characters
  while (fanLine.length() < 16) {
    fanLine += " ";
  }
  
  lcd.print(fanLine);
  delay(10);
  
  // Re-enable interrupts
  interrupts();
  
  // Update last displayed values
  lastDisplayedTemp = currentTemp;
  lastDisplayedStatus = fanStatus;
}

// ------------------ Fan Control ------------------
void turnFanOn() {
  digitalWrite(DIRA, HIGH);
  digitalWrite(DIRB, LOW);
  analogWrite(ENABLE, 255); // Full speed
}

void turnFanOff() {
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
  digitalWrite(ENABLE, LOW);
}
