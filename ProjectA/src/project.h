#ifndef PROJECT_H
#define PROJECT_H

//Includes
#include <wiringPi.h>
//#include <vector>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <wiringPiSPI.h>
#include <wiringPiI2C.h>
#include <softPwm.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
//#include <iostream>
#include <signal.h>
#include <mcp3004.h>

//Define Buttons
#define STOP_ALARM 28
#define TOGGLE_MONITORING 29
#define CHANGE_INTERVAL 1
#define RESET 4
#define PIN 12345

//SPI SETTINGS
#define ADC_CHAN 0
#define DAC_CHAN 1
#define SPI_SPEED 1000000


//Function Definitions
int main(void);
int initGPIO(void);
int initSPI(void);
int initI2C(void);
void CleanUp(int sig);

#endif
