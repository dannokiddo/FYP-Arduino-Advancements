#include <IRremote.h>

#define IRtransPin 4

void setup() {
  IrSender.begin(IRtransPin);
}

void loop() {
  IrSender.sendNEC(0x9C31057C, 32);
  //delay(1000);
}