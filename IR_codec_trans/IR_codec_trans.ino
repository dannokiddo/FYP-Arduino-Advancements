/*
MIT License
Copyright 2021 Michael Schoeffler (https://www.mschoeffler.com)
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/*
 * This is an example source code of a tutorial on how to do IR communication with the Arduino Uno. 
 * This program is for a sending Arduino Uno that sends an IR signal each second.
 */

#include <IRremote.h> // >v3.0.0
                                            
#define PIN_SEND 3

void setup()  
{  
  IrSender.begin(PIN_SEND); // Initializes IR sender
}  
                               
void loop()  
{  
  //  IrSender.sendNEC(0xFC03EF00, 0x34, true, 0); // the address 0x0102 with the command 0x34 is sent 
  IrSender.sendNEC(0xFC03EF00, 0x34, true, 32); // the address 0x0102 with the command 0x34 is sent 
  delay(1000); // wait for one second
}  