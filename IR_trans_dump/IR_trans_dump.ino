#include <IRremote.h>

#define transpin 3

void setup() {
  // put your setup code here, to run once:
  IrSender.begin(transpin);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*IrSender.sendNEC(0xEF00, 0x3, 1);
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
  delay(1000);*/

  // Fan
  /*IrSender.sendNEC(0x10, 0x12, 1);    // 1
  delay(2000);
  IrSender.sendNEC(0x10, 0x16, 1);    // 2
  delay(2000);
  IrSender.sendNEC(0x10, 0x1A, 1);    // 3
  delay(2000);
  IrSender.sendNEC(0x10, 0x13, 1);    // 1hr
  delay(2000);
  IrSender.sendNEC(0x10, 0x18, 1);    // 2hr
  delay(2000);*/

  // AC
  /*uint32_t tRawDataOn[]={0x807F807F, 0x37C807F, 0x6A9504FB};    // On
  IrSender.sendPulseDistanceWidthFromArray(38, 550, 1650, 550, 1650, 550, 600, &tRawDataOn[0], 95, 
    PROTOCOL_IS_LSB_FIRST, 100, 1);
  delay(2000);

  uint32_t tRawDataUp[]={0x807F807F, 0x37C817E, 0x6A9504FB};    // Up Temp
  IrSender.sendPulseDistanceWidthFromArray(38, 600, 1600, 550, 1650, 550, 550, &tRawDataUp[0], 95,
    PROTOCOL_IS_LSB_FIRST, 500, 1);
  delay(2000);

  uint32_t tRawDataDown[]={0x7F807F, 0x37C81FE, 0x6A9504FB};      // Down Temp
  IrSender.sendPulseDistanceWidthFromArray(38, 550, 1650, 550, 1650, 550, 550, &tRawDataDown[0], 95,
    PROTOCOL_IS_LSB_FIRST, 500, 1);
  delay(2000);

  uint32_t tRawDataTimer[]={0x807FD0AF, 0x37C837C, 0x6A9504FB};    // Timer +1
  IrSender.sendPulseDistanceWidthFromArray(38, 550, 600, 550, 1650, 550, 550, &tRawDataTimer[0], 95, 
    PROTOCOL_IS_LSB_FIRST, 500, 1);
  delay(2000);

  uint32_t tRawDataOff[]={0x807F807F, 0x27D807F, 0x6A9504FB};    // Off
  IrSender.sendPulseDistanceWidthFromArray(38, 500, 1700, 550, 1650, 550, 550, &tRawDataOff[0], 95, 
    PROTOCOL_IS_LSB_FIRST, 500, 1);
  delay(2000);*/

  // Fan One Home
  IrSender.sendNEC(0x0, 0x1C, 1);
  delay(1000);
  IrSender.sendNEC(0x0, 0x16, 3);
  delay(1000);
  IrSender.sendNEC(0x0, 0x40, 3);
  delay(1000);
  IrSender.sendNEC(0x0, 0x1C, 1);
  delay(1000);
}
