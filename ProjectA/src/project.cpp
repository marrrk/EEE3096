/* EEE3096S Embedded Systems 2 - 2019
    Project A;  Enviromental Logger
	By Mark Njoroge and Junior Makgoe
	NJRMAR003, MKGPUL005
*/

#include "project.h"

//Global Variables
long lastInterruptTime = 0;



//initialising GPIO
int  initGPIO(void){
	//Setup WiringPi
	wiringPiSetup();

	

	//Setting up Buttons
	pinMode(RESET,INPUT);
	pullUpDnControl(RESET,PUD_UP);

	pinMode(CHANGE_INTERVAL,INPUT);
	pullUpDnControl(CHANGE_INTERVAL,PUD_UP);


	//Setting up Interrupts for each button/
//	wiringPiISR(RESET,INT_EDGE_BOTH,reset);
//	wiringPiISR(CHANGE_INTERVAL,INT_EDGE_BOTH,changeInterval);


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



