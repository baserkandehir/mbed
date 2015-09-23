/*   Brushless motor control library with l293d
*
*    @author: Baser Kandehir 
*    @date: July 16, 2015
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
*/

#include "brushlessController_L293D.h"

/* Digital Outputs */
DigitalOut en1(p18);
DigitalOut en2(p19);
DigitalOut en3(p20);
DigitalOut out1(p21);
DigitalOut out2(p22);
DigitalOut out3(p24);


/* Brushless motor control function 
*
* @brief: This function can control a brushless motor with L293D.
* By changing steps of the brushless motor one after another, motor
* can be controlled in either direction.
*
* @variables: 
*   dir: Direction of the motor
*   delay_time: Time delay between steps
*   stepNum: Number of steps
*/
void brushlessControl(bool dir, int delay_time, int stepNum)
{
    /* Digital outputs are initially zero */
    out1 = 0; out2 = 0; out3 = 0;
    
    /* Enable the outputs initially */
    en1 = 1; en2 = 1; en3 = 1; 
    
    int stepCount = 0; // Number of step counts
    
    /* Magical data_array to drive the brushless motor */ 
    // HIGH:1, LOW:0, HIGH-Z:2
    char data_array[]={1,2,0, 1,0,2, 2,0,1, 0,2,1, 0,1,2, 2,1,0};
    char data_arrayInv[]={0,1,2, 0,2,1, 2,0,1, 1,0,2, 1,2,0, 2,1,0};
    
    if(dir==1)      // if dir = 1, reverse the motor direction
        for(int k=0; k<18; k++)
            data_array[k] = data_arrayInv[k];      
    
    for (int i=0; i<stepNum; i++)
    {
        i%=6;  // Steps will be repeated at mod6
        out1 = data_array[3*i];    (data_array[3*i]   == 2)?(en1 = 0):(en1 = 1);   
        out2 = data_array[3*i+1];  (data_array[3*i+1] == 2)?(en2 = 0):(en2 = 1); 
        out3 = data_array[3*i+2];  (data_array[3*i+2] == 2)?(en3 = 0):(en3 = 1); 
        wait_ms(delay_time);   
        if(++stepCount == stepNum) break;    
    }   
}

/* Brushless motor one step movement function 
* 
* @brief: This function can move the motor one step in either direction
* It takes the previous step as a parameter and uses it to go next.
*
* @variables: 
*   dir: Direction of the motor
*   delay_time: Time delay between steps
*   prevStep: Previous step of the motor
*/
void oneStep(bool dir, int delay_time, int* prevStep)
{
    /* Digital outputs are initially zero */
    out1 = 0; out2 = 0; out3 = 0;
    
    /* Enable the outputs initially */
    en1 = 1; en2 = 1; en3 = 1; 
    
    /* Magical data_array to drive the brushless motor */ 
    // HIGH:1, LOW:0, HIGH-Z:2
    char data_array[]={1,2,0, 1,0,2, 2,0,1, 0,2,1, 0,1,2, 2,1,0};
      
    if(dir==1)
        *prevStep = *prevStep + 1;    // Increase prevStep for next step || WARNING: *prevStep++ did not work
    else 
        *prevStep = *prevStep - 1; 
        
    int i = *prevStep;
        
    if (i<0)   // Mod operation for the negative number
    {
        i*=(-1); i%=6; i=6-i; 
        i%=6;  // if i=6, it should be zero.   
    }   
    else 
        i%=6;   // Steps will be repeated at mod6
    
    out1 = data_array[3*i];    (data_array[3*i]   == 2)?(en1 = 0):(en1 = 1);   
    out2 = data_array[3*i+1];  (data_array[3*i+1] == 2)?(en2 = 0):(en2 = 1); 
    out3 = data_array[3*i+2];  (data_array[3*i+2] == 2)?(en3 = 0):(en3 = 1);   
    wait_ms(delay_time);  
}
