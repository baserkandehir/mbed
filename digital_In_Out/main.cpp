/*   Digital input output example on LPC1768 
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
*    This a basic program that uses digital input-output functions.
*    It counts how many times the button is pressed and displays the 
*    resulting value as 4-bit binary on LPC1768 leds. 
*     
*    @connections: Pull up button with 10k resistor is connected to pin 18.   
*     
*/

#include "mbed.h"

/* Activate on board LEDs */
DigitalOut LPC_led1(LED1);
DigitalOut LPC_led2(LED2);
DigitalOut LPC_led3(LED3);
DigitalOut LPC_led4(LED4);

DigitalIn button(p18);  // button (pull-up) is connected to P18

/* Function prototype */
void ledCounter(int value);

int count=0;

int main()
{
    while(1)
    {
       if(button.read()==0)           // if the button is pressed
       {   
           while(button.read()==0);   // wait until release
           wait_ms(20);               // button debounce
           count++;                   // count up
           count%=16;                 // count (mod 16)
       }
       ledCounter(count);             // display the count 
    }
}

// This function can display value(mod 16) as binary on LPC1768
// For example =  12 = (1010) led2 and led4 will be ON, led1 and led3 will be OFF.
void ledCounter(int value)
{
    LPC_led1=value%2;
    LPC_led2=(value>>1)%2;
    LPC_led3=(value>>2)%2;
    LPC_led4=(value>>3)%2;
}
