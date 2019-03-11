// Name:  Jared Peter-Contesse & Spencer Shaw
// Lab 5
// Description: 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Custum delay lib
#include "defs.h"
#include "hardware.h"
#include "kill.h"


#define RAND_SEED (time(NULL))

#define STATE_SCAN (1)
#define STATE_TARGET (2)
#define STATE_ARM (3)
#define STATE_ROTATE (4)
#define STATE_REPOSITION (5)

#define ROTATE_360_COUNT (500)
#define ROTATE_MIDDLE_SENSOR_COUNT (20)
#define ROTATE_FAR_SENSOR_COUNT (20)

#define DISTANCE_SENSOR_THRESHOLD (25)
#define LINE_SENSOR_THRESHOLD (170)

#define MOTOR_TURN_BACKWARDS_SPEED (-50)
#define MOTOR_TURN_FORWARD_SPEED (50)
#define MOTOR_FORWARD_SPEED (50)

#define REPOSITION_DISTANCE (500)


u08 scan_area(u08 triggered_sensor);
void init_rotate(u08 triggered_sensor);
void init_forward();
u08 forward_until_line();
u08 forward_for(u08 triggered_sensor);
void pick_random_direction(u08 triggered_sensor);
void rotate_direction(u08 triggered_sensor);


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
	start_screen(lab5_str);

	while(TRUE){
		switch(state){
			case STATE_SCAN:
				if(scan_area(triggered_senser) > 0){
					state = STATE_REPOSITION;
				}else{
					state = STATE_TARGET;
				}
				break;
			case STATE_TARGET:
				triggered_senser = forward_until_line();
				state = STATE_ARM;
				break;
			case STATE_ARM:
				//actuate_arm();
				state = STATE_ROTATE;
				break;
			case STATE_ROTATE:
				rotate_direction(triggered_senser);
				state = STATE_SCAN;
				break;
			case STATE_REPOSITION:
				pick_random_direction(triggered_senser); 
				if(forward_for(triggered_senser) > 0){
					state = STATE_ROTATE;
				}else{
					state = STATE_SCAN;
				}
				break;
			default:
				state = STATE_SCAN;
				break;
		}
	}
    return 27;
}

// Rotate while scanning for objects
u08 scan_area(u08 triggered_sensor){
	int degree_count = 0;

	init_rotate(triggered_sensor);
	while(1){
		if(read_distance_sensor() < DISTANCE_SENSOR_THRESHOLD){
			halt();
			return 0;
		}
		if(degree_count > ROTATE_360_COUNT){
			return 1;
		}
		degree_count++;
	}
	return 2;
}

// Begin Rotating
void init_rotate(u08 triggered_sensor){
	if((triggered_sensor == LINE_SENSOR_FAR_RIGHT) || (triggered_sensor == LINE_SENSOR_RIGHT)){
		motors(MOTOR_TURN_BACKWARDS_SPEED, MOTOR_TURN_FORWARD_SPEED);
	}else{
		motors(MOTOR_TURN_FORWARD_SPEED, MOTOR_TURN_BACKWARDS_SPEED);
	}
}

// Forward
void init_forward(){
	motors(MOTOR_FORWARD_SPEED, MOTOR_FORWARD_SPEED);
}

// Move Forward while looking for line
u08 forward_until_line(){
	line_data4_t line_data;
	init_forward();
	while(1){
		read_line_sensor4(&line_data);
		if(line_data.far_left > LINE_SENSOR_THRESHOLD){
			halt();
			return LINE_SENSOR_FAR_LEFT;
		}else if(line_data.left > LINE_SENSOR_THRESHOLD){
			halt();
			return LINE_SENSOR_LEFT;
		}else if(line_data.right > LINE_SENSOR_THRESHOLD){
			halt();
			return LINE_SENSOR_RIGHT;
		}else if(line_data.far_right > LINE_SENSOR_THRESHOLD){
			halt();
			return LINE_SENSOR_FAR_RIGHT;
		}
	}
	return 0;
}


// After reaching Line, Rotate slightly
void rotate_direction(u08 triggered_sensor){
	init_rotate(triggered_sensor);
	switch(triggered_sensor){
		case LINE_SENSOR_FAR_RIGHT:
			delay_ms(ROTATE_FAR_SENSOR_COUNT);
			break;
		case LINE_SENSOR_RIGHT:
			delay_ms(ROTATE_MIDDLE_SENSOR_COUNT);
			break;
		case LINE_SENSOR_LEFT:
			delay_ms(ROTATE_MIDDLE_SENSOR_COUNT);
			break;
		case LINE_SENSOR_FAR_LEFT:
			delay_ms(ROTATE_FAR_SENSOR_COUNT);
			break;
		default:
			break;
	}
	halt();
	return;
}

void pick_random_direction(u08 triggered_sensor){
	float rand_float = (rand()/RAND_MAX)*500;
	int rand_int = (int) rand_float;
	init_rotate(triggered_sensor);
	delay_ms(rand_int);
	return;
}

u08 forward_for(u08 triggered_sensor){
	line_data4_t line_data;
	int count = 0;
	init_forward();

	while(count < REPOSITION_DISTANCE){
		read_line_sensor4(&line_data);
		if(line_data.far_left > LINE_SENSOR_THRESHOLD){
			halt();
			return 1;
		}else if(line_data.left > LINE_SENSOR_THRESHOLD){
			halt();
			return 1;
		}else if(line_data.right > LINE_SENSOR_THRESHOLD){
			halt();
			return 1;
		}else if(line_data.far_right > LINE_SENSOR_THRESHOLD){
			halt();
			return 1;
		}
		count++;
	}
	return 0;
}
