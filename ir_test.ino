#include <IRremote.h>

#define IR_RECEIVE_PIN 8  // Connect IR receiver OUT pin here

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("IR Receiver Test - Press a remote button");
}

void loop() {
  if (IrReceiver.decode()) {
    Serial.print("IR Code: 0x");
    Serial.println(IrReceiver.decodedIRData.command, HEX);
    IrReceiver.resume(); // Ready to receive next command
  }
}
