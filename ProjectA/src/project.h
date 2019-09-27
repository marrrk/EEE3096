#ifndef PROJECT_H
#define PROJECT_H

//Includes
#include <wiringPi.h>
//#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPiSPI.h>
#include <wiringPiI2C.h>
#include <softPwm.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
//#include <iostream>
#include <signal.h>

//Define Buttons
#define STOP_ALARM
#define TOGGLE_MONITORING
#define CHANGE_INTERVAL 1
#define RESET 4

//SPI SETTINGS
#define ADC_CHAN 0
#define DAC_CHAN 1
#define SPI_SPEEED 1


//Function Definitions
int main(void);
int initGPIO(void);
int initSPI(void);



#endif
