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

//constant difinitions
#define STOP_ALARM 28
#define TOGGLE_MONITORING 29
#define CHANGE_INTERVAL 1
#define RESET 4
#define PIN 12345
#define BUZZER 24
const char RTCAddr = 0x6f;
const char SEC = 0x00;
const char MIN = 0x01;
const char HOUR = 0x02;


//SPI SETTINGS
#define ADC_CHAN 0
#define DAC_CHAN 1
#define SPI_SPEED 1000000


//Function Definitions
int main(void);
int initGPIO(void);
int toSeconds(int ss, int mm, int hh);
void to24Hr(void);
void CleanUp(int sig);

#endif
