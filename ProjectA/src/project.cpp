/* EEE3096S Embedded Systems 2 - 2019
    Project A;  Enviromental Logger
	By Mark Njoroge and Junior Makgoe
	NJRMAR003, MKGPUL005
*/

#include "project.h"

//Global Variables
long lastInterruptTime = 0;
bool alarmsound = false;
bool start = false;
int interval;
int resetTime;


void alarm_stop(void){
	long interrupttime = millis();
	if (interrupttime - lastInterruptTime > 200){
		printf("stopping alarm\n");
		alarmsound = false;
		}
	lastInterruptTime = interrupttime;
}

void changeInterval(void){
	long interrupttime = millis();
	if (interrupttime - lastInterruptTime > 200){
		if (interval == 5){
		printf("reading every 1 second\n");
		interval = 1;
		}
		else if (interval == 1){
		printf("reading every 3 second\n");
		interval = 3;
		}

		else {
		printf("reading every 5 second\n");
		interval = 5;
		}
	}
	lastInterruptTime = interrupttime;
}

void reset(void){
	long interrupttime = millis();
	if (interrupttime - lastInterruptTime > 200){
		printf("resetting\n");
		resetTime=0;
		}
	lastInterruptTime = interrupttime;
}

void start_stop_isr(void){
	long interrupttime = millis();
	if (interrupttime - lastInterruptTime > 200){
		if(start == true){
			printf("monotoring stopped");
			start = false;
		}
		else{
			printf("starting monotoring");
			start = true;
		}
	}
	lastInterruptTime = interrupttime;
}



//initialising GPIO
int  initGPIO(void){
	//Setup WiringPi
	wiringPiSetup();

	

	//Setting up Buttons
	pinMode(RESET,INPUT);
	pullUpDnControl(RESET,PUD_UP);

	pinMode(CHANGE_INTERVAL,INPUT);
	pullUpDnControl(CHANGE_INTERVAL,PUD_UP);

	pinMode(STOP_ALARM,INPUT);
	pullUpDnControl(STOP_ALARM,PUD_UP);

	pinMode(TOGGLE_MONITORING,INPUT);
	pullUpDnControl(TOGGLE_MONITORING,PUD_UP);

	//Setting up Interrupts for each button/
	wiringPiISR(RESET,INT_EDGE_BOTH,reset);
	wiringPiISR(CHANGE_INTERVAL,INT_EDGE_BOTH,changeInterval);
	wiringPiISR(STOP_ALARM,INT_EDGE_BOTH,alarm_stop);
	wiringPiISR(TOGGLE_MONITORING,INT_EDGE_BOTH,start_stop_isr);
	//Setting up Communications
//	wiringPiSPISetup(ADC_CHAN,SPI_SPEED);
	//TODO - DAC setup



	return 0;
}


//read from ADC
void ReadADC(void) {
	


}


//main function
int main(void) {
	printf("test\n");
}



