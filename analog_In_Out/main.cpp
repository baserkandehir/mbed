/*   Analog input output example on LPC1768 
*
*    @author: Baser Kandehir 
*    @date: July 9, 2015
*    @license: MIT license
*     
*   Copyright (c) 2015, Baser Kandehir, baser.kandehir@ieee.metu.edu.tr
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in
*   all copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*   THE SOFTWARE.
*
*    @description of the program: 
*
*    This a basic program that uses analog input-output functions.
*    Analog input is read from a potentiometer and written to a led
*    and that ADCdata is send to a terminal via a bluetooth or FTDI 
*    chip. There exist a delay between the readings in order to see 
*    ADCdata change from the terminal. 
*    
*    @connections:
*-------------------------------------------------------------- 
*    |LPC1768|        |Peripherals|
*    Pin 13 --------> (TX) RX pin of the FTDI or Bluetooth etc.
*    Pin 14 --------> (RX) TX pin of the FTDI or Bluetooth etc.
*    Pin 20 -------->  Potentiometer second pin (middle one)  
*    Pin 18 -------->  An LED (with or without resistor - both works)
*    GND ----------->  GND of any peripheral
*    VOUT (3.3 V) -->  VCC of the FTDI or Bluetooth etc.
*---------------------------------------------------------------
*   Note: Potentiometer is simply a mechanical variable resistance.
*   By connecting the first and third pins of the potentiometer 
*   to VCC and GND and turning the potentiometer, one can obtain
*   a variable analog voltage from the middle pin of the pot.
*   
*/

#include "mbed.h"

AnalogIn input(p20);          // a potentiometer is connected to pin 20
AnalogOut led(p18);           // an Led is connected to p18 for analog output operation
Serial ftdi(p13,p14);         // setup terminal link
int ADCdata;

int main()
{
    while(1)
    {
        ADCdata=input.read_u16();                     // 16 bit input value
        led.write_u16(ADCdata);                       // write this value to led
        ftdi.printf("ADC data is: %d \r\n",ADCdata);  // Send ADC data to terminal
        wait(0.2);
    }    
}
