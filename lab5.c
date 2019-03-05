// Name:  Jared Peter-Contesse & Spencer Shaw
// Lab 4 Part 1
// Description: 

#include <stdio.h>

// Custum delay lib
#include "hardware.h"
#include "kill.h"


char *lab5_str = "Lab5, press to start";

int main(void)
{
    init();
    start_screen(lab5_str);

    kill();

    // line_data_t line_data;
    // motor_command_t motors;
    // motors.left = 0;
    // motors.right = 0;
    // set_motors(motors);

    // while((get_btn() == 0) && (get_btn2() == 0)){
    //     delay_ms(1);
    // }

    // while (1)   
    // {
    // 	line_data = read_line_sensor();
    //     motors = compute_proportional(line_data.left, line_data.right);
    //     set_motors(motors);

    //     print_4(line_data.left, motors.left, line_data.right, motors.right);
    //     delay_ms(DELAY_MS);
    // }

    return 27;
}
