/*   Calculating Roll, Pitch and Yaw angles from IMU
*
*    @author: Baser Kandehir 
*    @date: August 5, 2015
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
*   Program can calculate roll, pitch and yaw angles from the raw data that comes 
*   from IMU. Yaw angle is compensated for tilt. All the angles are sent to the matlab
*   for further processing.
*     
*/

#include "mbed.h"
#include "HMC5883L.h"
#include "MPU6050.h"
#include "ledControl.h"

Serial pc(USBTX,USBRX);    
MPU6050 mpu6050;           
HMC5883L hmc5883l;    
Ticker toggler1;
Ticker filter;   
Ticker compass;        

void toggle_led1();
void toggle_led2();
void compFilter();
void tiltCompensatedAngle();

float pitchAngle = 0;
float rollAngle = 0;
float yawAngle = 0;

int main() 
{
    pc.baud(9600);                                 // baud rate: 9600
    mpu6050.whoAmI();                              // Communication test: WHO_AM_I register reading 
    mpu6050.calibrate(accelBias,gyroBias);         // Calibrate MPU6050 and load biases into bias registers
    mpu6050.init();                                // Initialize the sensor
    hmc5883l.init();
    filter.attach(&compFilter,    0.005);              // Call the complementaryFilter func.  every 5 ms (200 Hz sampling period)
    compass.attach(&tiltCompensatedAngle, 0.015);      // Call the tiltCompensatedAngle func. every 15 ms (75 Hz sampling period)
    while(1) 
    {
        pc.printf("%.1f,%.1f,%.1f\r\n",rollAngle,pitchAngle,yawAngle);  // send data to matlab
        wait_ms(40);
    }
}

void toggle_led1() {ledToggle(1);}
void toggle_led2() {ledToggle(2);}

/* This function is created to avoid address error that caused from Ticker.attach func */ 
void compFilter() {mpu6050.complementaryFilter(&pitchAngle, &rollAngle);}

/* Tilt compensated compass data */
// Works well for tilt in +/- 50 deg range
void tiltCompensatedAngle() 
{   
    float mag_Data[3], Xh, Yh;
    hmc5883l.readMagData(mag_Data);
   /* 
    mag_Data[0] = mag_Data[0] - 4.8;  // Magnetometer needs a calibration
    mag_Data[1] = mag_Data[1] - 8.6;
   */
    Xh = mag_Data[0] * cos(rollAngle*PI/180) - mag_Data[2] * sin(rollAngle*PI/180) ;
    
    Yh = mag_Data[0] * sin(pitchAngle*PI/180) * sin(rollAngle*PI/180) + 
    mag_Data[1] * cos(pitchAngle*PI/180) -
    mag_Data[2] * sin(pitchAngle*PI/180) * cos(rollAngle*PI/180) ;

    /* Calculate the compensated heading angle */
    double heading = atan2(Yh, Xh);
    
    // After calculating heading declination angle should be added to heading which is the error of the magnetic field in specific location.
    // declinationAngle can be found here http://www.magnetic-declination.com/
    // For Ankara (my location) declinationAngle is ~5.5 degrees (0.096 radians)
    float declinationAngle = 0.096;
    heading += declinationAngle;
    
    // Correct for when signs are reversed.
    if(heading < 0)
        heading += 2*PI;
    
    // Check for wrap due to addition of declination.
    if(heading > 2*PI)
        heading -= 2*PI;
    
    /* Convert radian to degrees */
    heading = heading * 180 / PI;  
    
    yawAngle = heading; 
}
