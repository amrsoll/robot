#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include <math.h>

#define GRABBING_MOTOR_PORT 66
#define GRAB_SPEED 1
#define GRAB_ANGLE 1
uint8_t motor;

int setGrabbingMotor()
{
    ev3_search_tacho_plugged_in(GRABBING_MOTOR_PORT,0, &motor, 0 )
}

int release()
{

}
int grab()
{
    set_tacho_command( motor, TACHO_RESET );
    get_tacho_max_speed( motor, &max_speed );
    set_tacho_stop_action( motor, TACHO_COAST );
    tacho_speed_sp( motor, GRAB_SPEED );
    //multi_set_tacho_time_sp( motor, 500 );
    set_tacho_ramp_up_sp( motor, 0 );
    set_tacho_ramp_down_sp( motor, 0 );
    set_tacho_command_inx( motor, TACHO_RUN_FOREVER );

    set_tacho_command_inx( motor, TACHO_STOP );

}
