// Name:  Jared Peter-Contesse & Spencer Shaw
// Lab 5
// Description: 

#include <stdio.h>

// Custum delay lib
#include "defs.h"
#include "hardware.h"
#include "kill.h"


#define RAND_SEED (time(NULL))

#define STATE_SCAN 1
#define STATE_TARGET 2
#define STATE_ARM 3
#define STATE_ROTATE 4
#define STATE_REPOSITION 5

char *lab5_str = "Lab5";

int main(void)
{
	//Init Functions
	init();
	srand(RAND_SEED);
	halt();

	// Variables
	u08 state = STATE_SCAN;
	u08 triggered_senser = LINE_SENSOR_FAR_RIGHT;

	//Wait for Button Press
	start_screen(lab5_str){

	while(TRUE){
		switch(state){
			case STATE_SCAN:
				if(scan_area(triggered_senser) > 0){
					state = STATE_REPOSITION;
				}else{
					state = STATE_TARGET;
				}
			case STATE_TARGET:
				triggered_senser = forward_until_line();
				state = STATE_ARM;
			case STATE_ARM:
				actuate_arm();
			case STATE_ROTATE:
			case STATE_REPOSITION:
			default:
				state = STATE_SCAN;
		}
	}
    return 27;
}

