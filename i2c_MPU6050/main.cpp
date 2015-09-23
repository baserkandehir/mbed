/*   Getting Pitch and Roll Angles from MPU6050
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
*    First of all most of the credit goes to Kris Winer for his useful MPU6050 code.
*    I rewrite the code in my way using class prototypes and my comments. This program
*    can be a starter point for more advanced projects including quadcopters, balancing
*    robots etc. Program takes accelerometer and gyroscope data from the MPU6050 registers
*    and calibrates them for better results. Then uses this data is to obtain pitch and roll
*    angles and writes these angles to the terminal which mbed is connected to. 
*   
*    @connections:
*-------------------------------------------------------------- 
*    |LPC1768|        |Peripherals|
*    Pin 9 ---------> SDA of MPU6050
*    Pin 10 --------> SCL of MPU6050
*    GND -----------> GND of MPU6050
*    VOUT (3.3 V) --> VCC of MPU6050
*---------------------------------------------------------------
*-------------------------------------------------------------- 
*    |NUCLEO F411RE|  |Peripherals|
*    D14 -----------> SDA of MPU6050
*    D15 -----------> SCL of MPU6050
*    GND -----------> GND of MPU6050
*    VOUT (3.3 V) --> VCC of MPU6050
*---------------------------------------------------------------


*   Note: For any mistakes or comments, please contact me.
*/

#include "mbed.h"
#include "MPU6050.h"
#include "ledControl.h"

/* */

/* Defined in the MPU6050.cpp file  */
// I2C i2c(p9,p10);         // setup i2c (SDA,SCL)  

Serial pc(USBTX,USBRX);    // default baud rate: 9600
MPU6050 mpu6050;           // class: MPU6050, object: mpu6050 
Ticker toggler1;
Ticker filter;           

void toggle_led1();
void toggle_led2();
void compFilter();

float pitchAngle = 0;
float rollAngle = 0;

int main() 
{
    pc.baud(9600);                              // baud rate: 9600
    mpu6050.whoAmI();                           // Communication test: WHO_AM_I register reading 
    wait(1);
    mpu6050.calibrate(accelBias,gyroBias);      // Calibrate MPU6050 and load biases into bias registers
    pc.printf("Calibration is completed. \r\n");
    wait(0.5);
    mpu6050.init();                             // Initialize the sensor
    wait(1);
    pc.printf("MPU6050 is initialized for operation.. \r\n\r\n");
    wait_ms(500);
    
    while(1) 
    {
     
     /* Uncomment below if you want to see accel and gyro data */
        
//        pc.printf(" _____________________________________________________________  \r\n");
//        pc.printf("| Accelerometer(g) | ax=%.3f | ay=%.3f | az=%.3f                \r\n",ax,ay,az);
//        pc.printf("| Gyroscope(deg/s) | gx=%.3f | gy=%.3f | gz=%.3f                \r\n",gx,gy,gz);
//        pc.printf("|_____________________________________________________________  \r\n\r\n");
//        
//        wait(2.5);
                
        filter.attach(&compFilter, 0.005);    // Call the complementaryFilter func. every 5 ms (200 Hz sampling period)
        
        pc.printf(" _______________\r\n");
        pc.printf("| Pitch: %.3f   \r\n",pitchAngle);
        pc.printf("| Roll:  %.3f   \r\n",rollAngle);
        pc.printf("|_______________\r\n\r\n");
        
        wait(1);
     
    }
}

void toggle_led1() {ledToggle(1);}
void toggle_led2() {ledToggle(2);}

/* This function is created to avoid address error that caused from Ticker.attach func */ 
void compFilter() {mpu6050.complementaryFilter(&pitchAngle, &rollAngle);}
