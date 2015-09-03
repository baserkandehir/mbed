/*   Calculating Heading (Yaw Angle) from HMC5883L
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
*    @description of the program: Program takes the raw data from the HMC5883L 
*    digital compass sensor and calculates the heading angle. Writes the heading 
*    angle to the terminal every 1 second.
*    
*    @connections:
*-------------------------------------------------------------- 
*    |LPC1768|        |Peripherals|
*    Pin 9 ---------> SDA of HMC5883L
*    Pin 10 --------> SCL of HMC5883L
*    GND -----------> GND of HMC5883L
*    VOUT (3.3 V) --> VCC of HMC5883L
*---------------------------------------------------------------
*-------------------------------------------------------------- 
*    |NUCLEO F411RE|  |Peripherals|
*    D14 -----------> SDA of HMC5883L
*    D15 -----------> SCL of HMC5883L
*    GND -----------> GND of HMC5883L
*    VOUT (3.3 V) --> VCC of HMC5883L
*---------------------------------------------------------------
*
*/

#include "mbed.h"
#include "HMC5883L.h"
#include "ledControl.h"

Serial pc(USBTX,USBRX);    // default baud rate: 9600
HMC5883L hmc5883l;   

double Heading;

int main(void)
{
    hmc5883l.init();
    while(1) 
    {
        Heading = hmc5883l.getHeading();
        
        pc.printf(" _______________\r\n");
        pc.printf("| Heading: %.1f   \r\n", Heading);
        pc.printf("|_______________\r\n\r\n");
        
        wait(1);
    }
}
