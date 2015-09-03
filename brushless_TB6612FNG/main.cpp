/*   Controlling a brushless motor precisely with 2X TB6612FNG
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
*    @description of the program: 
*    
*    This a brushless motor control program. As an example it uses a timer to change the direction of the 
*    motor. When timer reaches 2 seconds, it changes the direction of movement and resets the timer. One can 
*    change the speed of the motor by changing the delay between brushless motor steps.
*
*    I learned the method from this website, check out the website for more information:
*
*    http://www.berryjam.eu/2015/04/driving-bldc-gimbals-at-super-slow-speeds-with-arduino/
*
*/

#include "mbed.h"
#include "brushlessController_TB6612FNG.h"
 
Timer changeDir;        // Timer for changing the direction 
bool dir = 0;           // Direction of movement
int delay = 1000;         // Delay between steps (us)

int main() 
{
    changeDir.start();  // Start timer
    while(1)
    {  
       if(changeDir.read() > 1)
        { 
            dir = !dir;
            changeDir.reset();
        }
        brushlessControl(dir, delay);  
    }
}
