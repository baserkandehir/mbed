/*   Interrupt, timer and ticker example on LPC1768 
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
*    This is an example program that shows how to use timer, ticker and interrupts. 
*    It uses LPC1768 board leds and a button to show how interrupts, timers and tickers 
*    are implemented to toggle leds. 
*    A ticker is an interrupt service routine which calls a function at periodic time intervals.
*    There are two tickers in the program. First ticker calls the toggle_led1 function every 100 ms
*    and second ticker calls the toggle_led2 function every 500 ms. A button is connected to show
*    how external interrupts work on the mbed LPC1768. Whenever user pushes the button, it pulls
*    down the pin 18 to 0V and external interrupt occurs at the falling edge of p18. Whenever an external
*    interrupt occurs, it calls the led3_debounced function to toggle led3. Also a timer is used to debounce 
*    the button by counting 20 ms. Debounce is required for buttons in order to prevent false readings. 
*     
*    @connections: Pull up button with 10k resistor is connected to pin 18.   
*     
*/

#include "mbed.h"

Ticker toggler1;            // periodic interrupt routines
Ticker toggler2;
Timer debounce;             // define debounce timer
InterruptIn button(p18);    // hardware interrupt on button - pin 18

/* IMPORTANT NOTE: any digital input can be an interrupt except pin 19 and pin 20 */

/* Interesting way of initialize all the 4 leds on the LPC1768 to an array named leds[]. */
DigitalOut leds[] = {(LED1), (LED2), (LED3), (LED4)};

/* Function Prototypes */
void toggle_led1();
void toggle_led2();
void led3_debounced();

int main() 
{ 
    /* Leds are initially OFF */
    for(int i=0; i<4 ;i++)
        leds[i]=0;            
   
    /* Start debounce timer */
    debounce.start();
    
    /* Attach functions to periodic interrupts */
    toggler1.attach(&toggle_led1,0.1);     // toggles led1 every 100 ms
    toggler2.attach(&toggle_led2,0.5);     // toggles led2 every 500 ms
    
    /* Call toggle_led3 falling edge of the button interrupt */
    button.fall(&led3_debounced);  
    while(1) 
    {
            
    }
}

void toggle_led1() {leds[0] = !leds[0];}
void toggle_led2() {leds[1] = !leds[1];}

// whenever user pushes the button, led3 toggles.
// button is debounced by waiting at least 20 ms.
void led3_debounced()
{
    if(debounce.read_ms()>20)      // only allow toggle after 20 ms
    {
        leds[2] = !leds[2];
        debounce.reset();          // restart timer after toggle    
    }         
}
