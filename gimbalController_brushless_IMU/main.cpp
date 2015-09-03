/*   Brushless gimbal controller with IMU (MPU050)
*
*    @author: Baser Kandehir 
*    @date: July 22, 2015
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
*    This a one axis gimbal control program that takes roll angle from an IMU 
*    and moves the gimbal brushless motor accordingly using PID algorithm. 
*
*    Microcontroller: LPC1768  
*    IMU: MPU6050
*    Motor driver: 2x TB6612FNG
*
*   Note: For any mistakes or comments, please contact me.
*/

#include "mbed.h"
#include "MPU6050.h"
#include "ledControl.h"
#include "brushlessController_TB6612FNG.h"

Serial pc(USBTX, USBRX);       // Create terminal link
MPU6050 mpu6050;               // mpu6050 object from MPU6050 classs
Ticker toggler1;               // Ticker for led toggling
Ticker filter;                 // Ticker for periodic call to compFilter funcçs 
Ticker gimbal;                 // Periodic routine for PID control of gimbal system
Ticker speed;                  // Periodic routine for speed control

/* Function prototypes */
void toggle_led1();
void toggle_led2();
void compFilter();
void gimbalPID();
void speedControl();

/* Variable declarations */
float pitchAngle = 0;
float rollAngle = 0;
bool dir;           // direction of movement
bool stop;          // to stop the motor
float delay;        // time delay between steps
float Kp = 10;
float Ki = 0.0001; 
float Kd = 5;
float set_point = 0;         // which angle camera should stay
float proportional = 0;
float last_proportional =0;
float integral = 0;
float derivative = 0;
float errorPID = 0;            // error is already declared at mbed libraries

int main()
{
    pc.baud(9600);                               // baud rate: 9600
    mpu6050.whoAmI();                            // Communication test: WHO_AM_I register reading 
    mpu6050.calibrate(accelBias,gyroBias);       // Calibrate MPU6050 and load biases into bias registers
    pc.printf("Calibration is completed. \r\n"); 
    mpu6050.init();                              // Initialize the sensor
    pc.printf("MPU6050 is initialized for operation.. \r\n\r\n");
   
    filter.attach(&compFilter, 0.005);       // Call the complementaryFilter func. every 5 ms (200 Hz sampling period)  
    gimbal.attach(&gimbalPID,  0.005);       // Same period with gimbalPID (important)  
    while(1)
    {          
        pc.printf("%.1f,%.1f\r\n",rollAngle,pitchAngle);
        wait_ms(40);
    }    
}

void toggle_led1() {ledToggle(1);}
void toggle_led2() {ledToggle(2);}

/* This function is created to avoid address error that caused from Ticker.attach func */ 
void compFilter() {mpu6050.complementaryFilter(&pitchAngle, &rollAngle);}

void gimbalPID()
{
    proportional = set_point - rollAngle;        
    integral += proportional; 
    derivative = proportional - last_proportional;
    last_proportional = proportional;
    
    errorPID = (Kp * proportional) + (Ki * integral) + (Kd * derivative); 
    (errorPID > 0)?(dir = 1):(dir = 0);
    
    /* errorPID is restricted between -400 and 400 */ 
    if(errorPID > 400)
        errorPID = 400;
    else if(errorPID < -400)
        errorPID = -400;   
   
    stop = 0;   
    delay = 0.1/abs(errorPID);      /* speed should be proportional to error, therefore time delay
                                     between steps should be inverse proportional to error.*/
                                      
    if (abs(errorPID)< Kp/2) stop = 1;  // 0.5 deg noise margin
         
    if(stop)  // if the gimbal is within noise margin, dont move.
        speed.detach();
    else    
        speed.attach(&speedControl, delay);      
}

void speedControl() 
{    
    brushlessControl(dir, 0);    
}