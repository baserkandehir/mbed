/*  Bluetooth(HC-06) example on LPC1768 
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
*    This is a very basic bluetooth program which sends data (a number in this case) 
*    through bluetooth module to a PC or smart phone (simply any device which has 
*    bluetooth connectivity) and increments the number every 500 ms and writes it 
*    to the screen with the regular printf function.
*
*    @connections:
*-------------------------------------------------------------- 
*    |LPC1768|        |Peripherals|
*    Pin 13 --------> (TX) RX pin of the Bluetooth module 
*    Pin 14 --------> (RX) TX pin of the Bluetooth  module
*    GND ----------->  GND of the Bluetooth module
*    VOUT (3.3 V) -->  VCC of the Bluetooth module
*---------------------------------------------------------------
*
*/

#include "mbed.h"

Serial bluetooth(p13,p14);    // p13: TX, p14: RX (LPC1768)
int i=0;

int main()
{
    bluetooth.baud(9600);
    bluetooth.printf("Hello world... \r\n");
    while(1)
    {
       i++;
       bluetooth.printf("Value of the i is: %d \r\n",i);
       wait(0.5);
    }    
}