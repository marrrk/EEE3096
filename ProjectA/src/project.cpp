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
int interval = 1000;
int resetTime;
double temperature;
double light;
double humidity;
double output;
uint8_t readings[3];

//clean up function
void CleanUp(int sig){
	printf("Cleaning up :)\n");

	//set output pins back to input i guess

	exit(0);
}


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
		if (interval == 5000){
		printf("reading every 1 second\n");
		interval = 1000;
		}
		else if (interval == 1000){
		printf("reading every 2 second\n");
		interval = 2000;
		}

		else {
		printf("reading every 5 second\n");
		interval = 5000;
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
			printf("Stop Monitoring\n");
			start = false;
		}
		else{
			printf("Start Monitoring\n");
			start = true;
		}
	}
	lastInterruptTime = interrupttime;
}



//initialising GPIO
int  initGPIO(void){
	//Setup WiringPi
	wiringPiSetup();
	mcp3004Setup(PIN,ADC_CHAN);


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
	//wiringPiSPISetup(ADC_CHAN,SPI_SPEED);

	//TODO - DAC setup



	return 0;
}


//read from ADC
void ReadADC(void) {
	humidity = analogRead(PIN);
	light = analogRead(PIN+1);
	temperature = analogRead(PIN+2);

	humidity = humidity*(3.3/1024);
//	light = light*(3.3/1024);
	temperature = temperature*(3.3/1024);
	temperature = temperature/0.01;


	output = (light/1023)*humidity;
}


//main function
int main(void) {
	printf("test\n");
	initGPIO();
	for (;;) {
	ReadADC();
	printf("the humidity is %.2f\n",humidity);
	printf("light reading: %.0f\n",light);
	printf("temp reading: %.1f\n",temperature);
	printf("DAC Output: %.2f\n",output);

	if ((output<0.65)| (output>2.65)) {
		alarmsound = true;
		printf("sounding alarm\n");

	}
	delay(interval);
	}


	return 0;
}



