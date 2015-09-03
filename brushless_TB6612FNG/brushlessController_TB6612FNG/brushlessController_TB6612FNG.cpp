/*   Brushless motor control library with TB6612FNG
*
*    @author: Baser Kandehir 
*    @date: July 17, 2015
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
*    @brief: 
*
*    This a brushless motor control library. Generally brushless motors can be controlled
*    using ESC s and motor drivers like L6234,L6235 etc. I previously control a brushless motor
*    with 2x L293D. Motor was a little bit shaking and its movement was not precise. The reason for that
*    was my way of driving the motor was a little bit wrong and max pwm freq. for L293D was restricted to
*    5 kHz. Now I am using 2x TB6612FNG which has fast response and supports max switching freq. up to 100 kHz.
*    The movement is really precise and results are remarkable. What makes this movement so precise is the way
*    we control pwm outputs. Before then I was just using digital outputs and it didnt give perfect results.
*    Now I am using the pwm method which creates sin wave like shapes at the outputs with 120 deg phase shift. 
*    I learned the method from this website, check out the website for more information:
*
*    http://www.berryjam.eu/2015/04/driving-bldc-gimbals-at-super-slow-speeds-with-arduino/
*
*/

#include "brushlessController_TB6612FNG.h"

PwmOut pwm[] = {(p21),(p22),(p24)};   // PWM outputs to drive brushless motor

int step[3];                          // Current steps of three pwm outputs
int sinArraySize;                     // Size of pwmSin array
int phaseShift;                       // Phase shift between outputs
bool firstTime = 1;

/* Magical data array to drive the brushless motor */
const int pwmSin[] = { // pwmSin data array consists of 360 samples
128, 132, 136, 140, 143, 147, 151, 155, 159, 162, 166, 170, 174, 178, 181, 185, 189, 192, 196, 200, 
203, 207, 211, 214, 218, 221, 225, 228, 232, 235, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 
248, 248, 249, 250, 250, 251, 252, 252, 253, 253, 253, 254, 254, 254, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 253, 252, 252, 251, 250, 250, 249, 248, 
248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 
248, 248, 249, 250, 250, 251, 252, 252, 253, 253, 253, 254, 254, 254, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 253, 252, 252, 251, 250, 250, 249, 248, 
248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 235, 232, 228, 225, 221, 218, 214, 211, 207, 
203, 200, 196, 192, 189, 185, 181, 178, 174, 170, 166, 162, 159, 155, 151, 147, 143, 140, 136, 132, 
128, 124, 120, 116, 113, 109, 105, 101, 97,  94,  90,  86,  82,  78,  75,  71,  67,  64,  60,  56, 
53,  49,  45,  42,  38,  35,  31,  28,  24,  21,  18,  17,  16,  15,  14,  13,  12,  11,  10,  9, 
8,   8,   7,   6,   6,   5,   4,   4,   3,   3,   3,   2,   2,   2,   1,   1,   1,   1,   1,   1, 
1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   3,   3,   3,   4,   4,   5,   6,   6,   7,   8, 
8,   9,   10,  11,  12,  13,  14,  15,  16,  17,  18,  17,  16,  15,  14,  13,  12,  11,  10,  9, 
8,   8,   7,   6,   6,   5,   4,   4,   3,   3,   3,   2,   2,   2,   1,   1,   1,   1,   1,   1, 
1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   3,   3,   3,   4,   4,   5,   6,   6,   7,   8, 
8,   9,   10,  11,  12,  13,  14,  15,  16,  17,  18,  21,  24,  28,  31,  35,  38,  42,  45,  49, 
53,  56,  60,  64,  67,  71,  75,  78,  82,  86,  90,  94,  97,  101, 105, 109, 113, 116, 120, 124
};

/* Brushless motor control function 
*
* @brief: This function can control a brushless motor PRECISELY with TB6612FNG.
* It initializes the 3 pwm outputs with 20 kHz frequency. Function takes data
* from the pwmSin array and gives the data to the 3 pwm outputs with 120 deg 
* phase shift. By doing so it can control a brushless motor precisely with 
* simple DC motor drivers.
*
* @variables: 
*   dir: Direction of the motor
*   delay_time (us): Time delay between steps
*/
void brushlessControl(bool dir, int delay_time)
{
    if(firstTime == 1)                  // do it only once (initialization process)
    {   
        for(int i=0; i<3; i++)     
            pwm[i].period(1/20000.0);   // 20 kHz pwm frequency (inaudible)
        
        sinArraySize = sizeof(pwmSin) / sizeof(int);   // 360 for this case
        phaseShift = sinArraySize / 3 ;                // 120 for the above case
        
        step[0] = 0;
        step[1] = step[0] + phaseShift;
        step[2] = step[1] + phaseShift; 
    
        firstTime = 0;
    }
           
    for(int k=0; k<3; k++) 
    {          
        /* PWM duty cycles are determined by the step numbers of the outputs,
        corresponding data is taken from pwmSin array */
        pwm[k] = pwmSin[step[k]]/255.0;       // Pwm takes values between 0 and 1.0 (float)   
        
        (dir == 1)? (step[k]++):(step[k]--);  // Motor direction control
        
        /* If array index exceeds the array size, start over */
        if(step[k] == sinArraySize - 1)   step[k] = 0; 
        if(step[k] < 0)                   step[k] = sinArraySize - 1;
    } 
    wait_us(delay_time);  
}