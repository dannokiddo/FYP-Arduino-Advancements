#include <IRremote.h>

#define transpin 3

void setup() {
  // put your setup code here, to run once:
  IrSender.begin(transpin);
}

void loop() {
  // put your main code here, to run repeatedly:
  IrSender.sendNEC(0xEF00, 0x3, 1);
  delay(1000);
  //IrSender.sendNEC(0xEF00, 0x7, 1);
  //delay(1000);
  IrSender.sendNEC(0xEF00, 0x1, 1);
  delay(1000);
  IrSender.sendNEC(0xEF00, 0x0, 1);
  delay(1000);
  IrSender.sendNEC(0xEF00, 0x2, 1);
  IrSender.sendNEC(0xEF00, 0x2, 1);
  IrSender.sendNEC(0xEF00, 0x2, 1);
  delay(1000);
}
