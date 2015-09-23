/*   Controlling a brushless motor with 2X L293D
*
*    @author: Baser Kandehir 
*    @date: July 16, 2015
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
*    Brushless motor control program with L293D. 
*    L293D is a simple, cheap standard DC motor driver and can be found easily.
*    But I should say that controlling brushless motors with L293D is not efficient 
*    and precise. Proper way to control brushless motor is using an ESC or drivers
*    like L6235. For the time being I have none of them, but I had a brushless motor that 
*    I wanted to control and see how it works. If you want to do the same, that program
*    is for you. 
*      
*    This guy did a great job by explaining how it is done:    
*    http://www.instructables.com/id/Arduino-CDROM-BLDC-Motor-Driver-Enhanced-Performan/
*
*   Note: For any mistakes or comments, please contact me.
*/

#include "mbed.h"
#include "brushlessController_L293D.h"

int prevStep = 0;

int main() 
{
    /* Example: After nine steps it will stop */
    //brushlessControl(1, 500, 9);  
      
    while(1) 
    {          
        oneStep(0, 500, &prevStep);   // One step forward wait 500ms   
        oneStep(1, 500, &prevStep);   // One step backward wait 500ms 
        
        /* 500 ms delay time is just for testing, you should reduce the delay time for better movement */ 
    }
}
