/*  Datalogging with Real Time Clock on LPC1768 
*
*    @author: Baser Kandehir 
*    @date: July 8, 2015
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
*    This program can log data that is read from RTC(real time clock) which
*    is embedded inside LPC1768. It saves the data to the LPC1768’s memory as 
*    “log.txt”. For this example program, the data is how many times the button 
*    is pressed and when it is pressed in terms of real time. It can also 
*    display the button count as 4-bit binary number using 4 leds on the LPC1768. 
*    There are two different time settings on the code. One can set the time from 
*    the terminal or manually by uncommenting the corresponding portion of code. 
*    Once the time is set, if there is a battery connected to real-time clock, 
*    RTC will continue to keep track of current time. In order not to reset the 
*    real-time to its initial value every time microcontroller is reset, it is 
*    neccessary to set the time once and delete the portion of code which sets 
*    the time and keep the rest.
*   
*    @connections:
*-------------------------------------------------------------- 
*    |LPC1768|        |Peripherals|
*    Pin 13 --------> (TX) RX pin of the FTDI or Bluetooth etc.
*    Pin 14 --------> (RX) TX pin of the FTDI or Bluetooth etc.
*    Pin 20 -------->  Pull-up connected button
*    GND ----------->  GND of any peripheral
*    VOUT (3.3 V) -->  Pull-up resistor
*---------------------------------------------------------------
*
*/

#include "mbed.h"
#include "ledCounter.h"

Serial ftdi(p13, p14);               // setup terminal link
LocalFileSystem local("local");      // define local file system
DigitalIn button(p20);               // pull-up button is connected to p20
     
int count=0;                         // button count 
struct tm t;                         // current time will be stored here

int main()
{   
   /* UNCOMMENT BELOW if you want to set the time from the terminal. */
/*
    ftdi.printf("Enter current date and time:\r\n");
    ftdi.printf("YYYY MM DD HH MM SS[enter]\r\n\r\n");    
    ftdi.scanf("%d %d %d %d %d %d", &t.tm_year, &t.tm_mon, &t.tm_mday, &t.tm_hour, &t.tm_min, &t.tm_sec);
    ftdi.printf("-------------------------- \r\n");
    
    t.tm_year = t.tm_year - 1900;   // adjust for tm structure required values
    t.tm_mon = t.tm_mon - 1;
    
    set_time(mktime(&t));           // set the time
*/  

    /* UNCOMMENT BELOW if you want to set time manually */
/*       
    t.tm_year = 2015;     // current year
    t.tm_mon = 7;         // current month
    t.tm_mday = 7;        // current day
    t.tm_hour = 15;       // current hour
    t.tm_min = 16;        // current minute
    t.tm_sec = 0;         // current second
    
    t.tm_year = t.tm_year - 1900;   // adjust for tm structure required values
    t.tm_mon = t.tm_mon - 1;
    
    set_time(mktime(&t));           // set the time
*/
      
    /* Once the time is set, if there is a battery (3V) connected to VB pin and GND, RTC will remember the exact time. */
    
    while(1) 
    {  
        time_t seconds = time(NULL);
        
        if(button.read()==0)          // if the button is pressed
        {   
            while(button.read()==0);  // wait until release
            wait_ms(20);              // button debounce
            count++;                  // count up
            
            FILE* Logfile = fopen ("/local/log.txt","a");                                  // open file for appending  
            
            ftdi.printf("Time: %s \rButton count: %d \r\n",ctime(&seconds),count);         // send data to terminal
            ftdi.printf("-------------------------- \r\n");
            
            fprintf(Logfile, "Time: %s \rButton count: %d \r\n",ctime(&seconds),count);    // save data to log.txt file
            fprintf(Logfile, "-------------------------- \r\n");
            fclose(Logfile);  
        }   
        ledCounter(count);         // display the count as binary
    }
}
