/* EEE3096S Embedded Systems 2 - 2019
    Project A;  Enviromental Logger
	By Mark Njoroge and Junior Makgoe
		NJRMAR003, MKGPUL005
*/

#include "project.h"
#include "CurrentTime.c"

//Global Variables
long lastInterruptTime = 0;
bool alarmsound = false;
bool start = false;
int interval = 1000;
int timer;
int hours,mins,secs;
int sysHrs,sysMins,sysSecs;
int sysInterval = 1;
double temperature;
double light;
double humidity;
double output;
//uint8_t readings[3];

//clean up function
void CleanUp(int sig){
	printf("Cleaning up :)\n");

	//set output pins back to input i guess

	exit(0);
}

//stops the alarm from going off
void alarm_stop(void){
	long interrupttime = millis();
	if (interrupttime - lastInterruptTime > 200){
		printf("Stopping Alarm\n");
		alarmsound = false;
		}
	lastInterruptTime = interrupttime;
}


//changes the interval at which data is measured
void changeInterval(void){
	long interrupttime = millis();
	if (interrupttime - lastInterruptTime > 200){
		if (interval == 5000){
		printf("reading every 1 second\n");
		interval = 1000;
		sysInterval = 1;
		}
		else if (interval == 1000){
		printf("reading every 2 second\n");
		interval = 2000;
		sysInterval = 2;
		}

		else {
		printf("reading every 5 second\n");
		interval = 5000;
		sysInterval = 5;
		}
	}
	lastInterruptTime = interrupttime;
}


//resets the time
void reset(void){
	long interrupttime = millis();
	if (interrupttime - lastInterruptTime > 200){
		printf("resetting\n");
		sysHrs = 0;
		sysMins = 0;
		sysSecs = 0;
		}
	lastInterruptTime = interrupttime;
}

//toggles monitoring
void start_stop_isr(void){
	long interrupttime = millis();
	if (interrupttime - lastInterruptTime > 200){
		if(start == true){
			printf("Monitoring Stopped\n");
			start = false;
		}
		else{
			printf("Monitoring Started\n");
			start = true;
		}
	}
	lastInterruptTime = interrupttime;
}



//initialising GPIO
int  initGPIO(void){
	//Setup WiringPi and ADC
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
	wiringPiISR(RESET,INT_EDGE_FALLING,reset);
	wiringPiISR(CHANGE_INTERVAL,INT_EDGE_FALLING,changeInterval);
	wiringPiISR(STOP_ALARM,INT_EDGE_FALLING,alarm_stop);
	wiringPiISR(TOGGLE_MONITORING,INT_EDGE_FALLING,start_stop_isr);

	//Setting up Communications
//	RTC = wiringPiI2CSetup(RTCAddr);



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

	if ((output<0.65) | (output>2.65)) {
		alarmsound = true;

	}
}


void incrementTimer(void) {
	if (sysSecs >= 59) {
		sysSecs = 0;
		if (sysMins >= 59) {
			sysMins = 0;
			if (sysHrs >= 23) {
				sysHrs = 0;
			}
			else sysHrs +=1 ;
		}
		else sysMins += 1;
	}
	else sysSecs = sysSecs + sysInterval;


}

//main function
int main(void) {
	signal(SIGINT,CleanUp);
	printf("test\n");
	initGPIO();
	for (;;) {
		if (start) {
			ReadADC();
			printf("the humidity is %.2f V\n",humidity);
			printf("light reading: %.0f\n",light);
			printf("temp reading: %.1f C\n",temperature);
			printf("DAC Output: %.2f V\n",output);
			if (alarmsound) {
				printf("alarm is sounding\n");

			}
		}
		hours = getHours();
		mins = getMins();
		secs = getSecs();
		printf("RTC is %d:%d:%d | System Time: %d:%d:%d \n",hours,mins,secs, sysHrs,sysMins,sysSecs);
		delay(interval);
		incrementTimer();
	}


	return 0;
}

int hexCompensation(int units){
	//convers hexadecimal number to decimal number
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
		units = 20 +unitsU;
	}
	else if (units >= 0x10){
		units = 10 + unitsU;
	}

	return units;

}


//converts decimal number to hexadecimal number
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



