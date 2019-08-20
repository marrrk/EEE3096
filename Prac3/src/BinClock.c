/*
 * BinClock.c
 * Jarrod Olivier
 * Modified for EEE3095S/3096S by Keegan Crankshaw
 * August 2019

 * MKGPUL005 NJRMAR003
 * Date 13/08/2019
*/

#include <wiringPi.h>
#include <signal.h> //for catching signals
#include <softPwm.h>
#include <wiringPiI2C.h>
#include <stdio.h> //For printf functions
#include <stdlib.h> // For system functions

#include "BinClock.h"
#include "CurrentTime.h"

//Global variables
int hours, mins, secs;
long lastInterruptTime = 0; //Used for button debounce
int RTC; //Holds the RTC instance
int outHr[8];
int outMin[8];
int HH,MM,SS;

//cleans up
void CleanUp(int sig){
	printf("Cleaning up\n");

	for (int i=0; i < 10 ; i++){
		digitalWrite(LEDS[i],0);
		pinMode(LEDS[i],INPUT);
	}

	for (int j=0; j < sizeof(BTNS)/sizeof(BTNS[0]); j++) {
		pinMode(BTNS[j],INPUT);
	}
	pinMode(SEC,OUTPUT);
	digitalWrite(SEC,0);
	pinMode(SEC,INPUT);

	exit(0);

}

void initGPIO(void){
	/* 
	 * Sets GPIO using wiringPi pins. see pinout.xyz for specific wiringPi pins
	 * You can also use "gpio readall" in the command line to get the pins
	 * Note: wiringPi does not use GPIO or board pin numbers (unless specifically set to that mode)
	 */
	printf("Setting up\n");
	wiringPiSetup(); //This is the default mode. If you want to change pinouts, be aware
	
	RTC = wiringPiI2CSetup(RTCAddr); //Set up the RTC
	
	//Set up the LEDS
	for(int i=0; i < sizeof(LEDS)/sizeof(LEDS[0]); i++){
	    pinMode(LEDS[i], OUTPUT);
	}
	
	//Set Up the Seconds LED for PWM
	//Write your logic here
	
	softPwmCreate(SECS,0,100);
	
	printf("LEDS done\n");
	
	//Set up the Buttons
	for(int j=0; j < sizeof(BTNS)/sizeof(BTNS[0]); j++){
		pinMode(BTNS[j], INPUT);
		pullUpDnControl(BTNS[j], PUD_UP);
	}
	
	//Attach interrupts to Buttons
	//Write your logic here
	wiringPiISR(BTNS[0],INT_EDGE_BOTH, hourInc);
	wiringPiISR(BTNS[1],INT_EDGE_BOTH, minInc);
	printf("BTNS done\n");
	printf("Setup done\n");
}


/*
 * The main function
 * This function is called, and calls all relevant functions we've written
 */
int main(void){
	signal(SIGINT,CleanUp);

	initGPIO();

	//Set random time (3:04PM)
	//You can comment this file out later
//	wiringPiI2CWriteReg8(RTC, HOUR, 0x13+TIMEZONE);
//	wiringPiI2CWriteReg8(RTC, MIN, 0x04);
//	wiringPiI2CWriteReg8(RTC, SEC, 0b10000000);
	toggleTime();
	// Repeat this until we shut down
	for (;;){
		
		
		//Fetch the time from the RTC
		//Write your logic here
		hours = wiringPiI2CReadReg8(RTC,HOUR);
		mins = wiringPiI2CReadReg8(RTC,MIN);
		secs = wiringPiI2CReadReg8(RTC,SEC) - 0b10000000;
		
		//Function calls to toggle LEDs
		//Write your logic here
		secPWM(secs);
		lightHours(hours);
		lightMins(mins);
		// Print out the time we have stored on our RTC
		printf("The current time is: %x:%x:%x\n", hours, mins, secs);

		//using a delay to make our program "less CPU hungry"
		delay(1000); //milliseconds
	}
	return 0;
}

/* functions that convert a decimal number to  a binary
 * each with bit is stored as an element of an array
 * first function handles the hours
 * second function handles the minutes
*/
int * toBinaryHr(int dec) {
	int i = 0;
	
	if (dec == 0) {
		for (int j = 0; j<8;j++) {
			outHr[j] = 0;
		}
	}
	else if ( dec == 1) {
		outHr[0] = 1;
		for (int j = 1; j< 8; j++) {
			outHr[j] = 0;
		}
	}
	else {
		while (dec > 0) {
			outHr[i] = dec % 2;
			dec = dec/2;
			i++;
		}

	}
	return outHr;
}

int *toBinaryMin(int dec) {
	int i = 0;
	if (dec == 0) {
		for (int j =0; j<8;j++) {
			outMin[j] = 0;
		}
	}
	else {
		while (dec >0) {
			outMin[i] = dec % 2;
			dec = dec/2;
			i++;
		}
	}
	return outMin;
}


/*
 * Change the hour format to 12 hours
 */
int hFormat(int hours){
	/*formats to 12h*/
	if (hours >= 24){
		hours = 0;
	}
	else if (hours > 12){
		hours -= 12;
	}
	return (int)hours;
}

/*
 * Turns on corresponding LED's for hours
 */
void lightHours(int units){
	// Write your logic to light up the hour LEDs her
	int num = hexCompensation(units);
	int nHr = hFormat(num);
	int *states = toBinaryHr(nHr);
	for (int i =0; i < 4; i++) {
		digitalWrite(LEDS[i],*(states+i));
	}
}

/*
 * Turn on the Minute LEDs
 */
void lightMins(int units){
	//Write your logic to light up the minute LEDs here
	int num = hexCompensation(units);
	int *states = toBinaryMin(num);
	for (int i =0; i<6;i++) {
		digitalWrite(LEDS[i+4],*(states+i));
	}
}

/*
 * PWM on the Seconds LED
 * The LED should have 60 brightness levels
 * The LED should be "off" at 0 seconds, and fully bright at 59 seconds
 */
void secPWM(int units){
	// Write your logic here
	int levels[59];
	int value = 0;
	double step = (100/60);
	for (int i = 0; i<60; i++) {
		levels[i] = value;
		value = value +step;
	}
	softPwmWrite(5,levels[hexCompensation(units)]);
}

/*
 * hexCompensation
 * This function may not be necessary if you use bit-shifting rather than decimal checking for writing out time values
 */
int hexCompensation(int units){
	/*Convert HEX or BCD value to DEC where 0x45 == 0d45 
	  This was created as the lighXXX functions which determine what GPIO pin to set HIGH/LOW
	  perform operations which work in base10 and not base16 (incorrect logic) 
	*/
	int unitsU = units%0x10;

	if (units >= 0x50){
		units = 50 + unitsU;
	}
	else if (units >= 0x40){
		units = 40 + unitsU;
	}
	else if (units >= 0x30){
		units = 30 + unitsU;
	}
	else if (units >= 0x20){
		units = 20 + unitsU;
	}
	else if (units >= 0x10){
		units = 10 + unitsU;
	}
	return units;
}


/*
 * decCompensation
 * This function "undoes" hexCompensation in order to write the correct base 16 value through I2C
 */
int decCompensation(int units){
	int unitsU = units%10;

	if (units >= 50){
		units = 0x50 + unitsU;
	}
	else if (units >= 40){
		units = 0x40 + unitsU;
	}
	else if (units >= 30){
		units = 0x30 + unitsU;
	}
	else if (units >= 20){
		units = 0x20 + unitsU;
	}
	else if (units >= 10){
		units = 0x10 + unitsU;
	}
	return units;
}


/*
 * hourInc
 * Fetch the hour value off the RTC, increase it by 1, and write back
 * Be sure to cater for there only being 23 hours in a day
 * Software Debouncing should be used
 */
void hourInc(void){
	//Debounce
	long interruptTime = millis();

	if (interruptTime - lastInterruptTime>200){
		printf("Interrupt 1 triggered, %x\n", hours);
		//Fetch RTC Time
		hours = wiringPiI2CReadReg8(RTC,HOUR);
		hours = hexCompensation(hours);
		//Increase hours by 1, ensuring not to overflow
		if (hours == 23){
			hours = 0;
		}
		else {
			hours = hours + 1;
		}
		//Write hours back to the RTC
		hours = decCompensation(hours);
		wiringPiI2CWriteReg8(RTC,HOUR,hours);
	}
	lastInterruptTime = interruptTime;
}

/* 
 * minInc
 * Fetch the minute value off the RTC, increase it by 1, and write back
 * Be sure to cater for there only being 60 minutes in an hour
 * Software Debouncing should be used
 */
void minInc(void){
	long interruptTime = millis();

	if (interruptTime - lastInterruptTime>200){
		printf("Interrupt 2 triggered, %x\n", mins);
		//Fetch RTC Time
		mins =  wiringPiI2CReadReg8(RTC,MIN);
		mins = hexCompensation(mins);
		//Increase minutes by 1, ensuring not to overflow
		if (mins == 59) {
			mins = 0;
		}
		else {
			mins = mins + 1;
		}
		//Write minutes back to the RTC
		mins = decCompensation(mins);
		wiringPiI2CWriteReg8(RTC,MIN,mins);
	}
	lastInterruptTime = interruptTime;
}

//This interrupt will fetch current time from another script and write it to the clock registers
//This functions will toggle a flag that is checked in main
void toggleTime(void){
	long interruptTime = millis();

	if (interruptTime - lastInterruptTime>200){
		HH = getHours();
		MM = getMins();
		SS = getSecs();

		HH = hFormat(HH);
		HH = decCompensation(HH);
		wiringPiI2CWriteReg8(RTC, HOUR, HH);

		MM = decCompensation(MM);
		wiringPiI2CWriteReg8(RTC, MIN, MM);

		SS = decCompensation(SS);
		wiringPiI2CWriteReg8(RTC, SEC, 0b10000000+SS);

	}
	lastInterruptTime = interruptTime;
}
