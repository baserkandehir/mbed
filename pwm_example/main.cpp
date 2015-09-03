/*   PWM motor driving example on LPC1768 
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
*    Program is written as an example for the PWM control on mbed LPC1768.
*    After making neccessary connections, this program can control speed 
*    and direction of a motor. Example code will increase motor speed to
*    its max in 5 seconds and decrease the motor speed from max to zero
*    in 5 seconds and it will keep doing this.    
*    
*    @connections:
*-------------------------------------------------------------- 
*    |LPC1768|        |Peripherals|
*    Pin 21 -------->  First input of the L293D
*    Pin 22 -------->  Second input of the L293D
*    GND ----------->  GND of any peripheral
*    VOUT (3.3 V) -->  VCC of the the motor driver
*---------------------------------------------------------------
*   Note: L293D is a very simple and useful motor driver that mostly used in
*   robotics. For detailed info and neccessary connections for L293D, please
*   refer to its datasheet.
*/

#include "mbed.h"

PwmOut PWM1(p21);     // pwm outputs
PwmOut PWM2(p22);

/* Function prototype */
void motorControl(bool dir, float pwmVal);

int main()
{
    while(1)
    {
        for(float i=0;i<1;i=i+0.01)  // increase speed from 0 to 1 in 5 seconds (50ms*100). 
        {
            motorControl(1,i);
            wait_ms(50);    
        }
        for(float i=1;i>0;i=i-0.01)  // decrease speed from 1 to 0 in 5 seconds (50ms*100).
        {
            motorControl(0,i);
            wait_ms(50);    
        }   
    }    
}

// This is a motor control function which controls the speed and direction of
// a motor. Motor drivers like L293D can be used.
void motorControl(bool dir, float pwmVal)
{
    PWM1.write(0);       // Duty cycles are initially zero   
    PWM2.write(0);   
    PWM1.period_ms(1);   // 1 kHz pwm frequency
    PWM2.period_ms(1);
    
    if(dir==1)               // forward direction
    {
        PWM1.write(pwmVal);        
        PWM2.write(0);   
    }
    else if(dir==0)         // reverse direction
    {
        PWM2.write(pwmVal);        
        PWM1.write(0);       
    }    
}