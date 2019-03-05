#ifndef KILL_H
#define KILL_H

#include "hardware.h"

#define FULL_SPEED_AHEAD (100)
#define ROTATE_SPEED (50)

void forward(int ticks){
    motors(FULL_SPEED_AHEAD, FULL_SPEED_AHEAD);
    while(1){
        delay_ms(50);
    }
    halt();
}

void rotate_90(){
    motors(ROTATE_SPEED, -ROTATE_SPEED);
    while(1){
        delay_ms(50);
    }
    halt();
}

void kill(){
    rotate_90();
    forward(500);
}

#endif