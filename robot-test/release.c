#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include <math.h>

#define Sleep( msec ) usleep(( msec ) * 1000 )
#define GRABBING_MOTOR_PORT 67
#define GRAB_SPEED 500
#define GRAB_ANGLE 2
uint8_t motor;
char s[ 256 ];
FLAGS_T state;


int setGrabbingMotor()
{
    // if(ev3_search_tacho_plugged_in(GRABBING_MOTOR_PORT,0, &motor[0], 0 ))
    // {
    //     printf("grabbing motor not found, abort, abot!");
    // } else
    // {
    //     printf("is this real life??");
    // }
}

int grab()
{
    if ( ev3_search_tacho_plugged_in(GRABBING_MOTOR_PORT,0, &motor, 0 )) {
		int max_speed;

		printf( "LEGO_EV3_M_MOTOR 1 is found, run for 5 sec...\n" );
		get_tacho_max_speed( motor, &max_speed );
		printf("  max speed = %d\n", max_speed );
		set_tacho_stop_action_inx( motor, TACHO_COAST );
		set_tacho_speed_sp( motor, -max_speed * 1 / 5 );
		set_tacho_time_sp( motor, 500 );
		set_tacho_ramp_up_sp( motor, 2000 );
		set_tacho_ramp_down_sp( motor, 2000 );
		set_tacho_command_inx( motor, TACHO_RUN_TIMED );
	} else {
		printf( "LEGO_EV3_M_MOTOR 1 is NOT found\n" );
	}
}

int release()
{
    // int max_speed;
    // multi_set_tacho_command_inx( motor, TACHO_RESET );
    // get_tacho_max_speed( motor[0], &max_speed );
    // printf("  max speed = %d\n", max_speed );
    // multi_set_tacho_command_inx( motor, TACHO_RESET );
    // get_tacho_max_speed( motor[0], &max_speed );
    // multi_set_tacho_stop_action_inx( motor, TACHO_COAST );
    // multi_set_tacho_speed_sp( motor, max_speed * 1 / 3 );
    // //multi_set_tacho_time_sp( motor, 500 );
    // multi_set_tacho_ramp_up_sp( motor, 0 );
    // multi_set_tacho_ramp_down_sp( motor, 0 );
    // multi_set_tacho_command_inx( motor, TACHO_RUN_FOREVER );
    // sleep(1);
    // multi_set_tacho_command_inx( motor, TACHO_STOP );


	if ( ev3_search_tacho_plugged_in(GRABBING_MOTOR_PORT,0, &motor, 0 )) {
		int max_speed;

		printf( "LEGO_EV3_M_MOTOR 1 is found, run for 5 sec...\n" );
		get_tacho_max_speed( motor, &max_speed );
		printf("  max speed = %d\n", max_speed );
		set_tacho_stop_action_inx( motor, TACHO_COAST );
		set_tacho_speed_sp( motor, max_speed * 1 / 5 );
		set_tacho_time_sp( motor, 500 );
		set_tacho_ramp_up_sp( motor, 2000 );
		set_tacho_ramp_down_sp( motor, 2000 );
		set_tacho_command_inx( motor, TACHO_RUN_TIMED );

///////////   RUN TO REAL POSITION

		// /* Wait tacho stop */
		// Sleep( 100 );
		// do {
		// 	get_tacho_state_flags( motor, &state );
		// } while ( state );
		// printf( "run to relative position...\n" );
		// set_tacho_speed_sp( motor, max_speed / 2 );
		// set_tacho_ramp_up_sp( motor, 0 );
		// set_tacho_ramp_down_sp( motor, 0 );
		// set_tacho_position_sp( motor, 90 );
		// for ( i = 0; i < 8; i++ ) {
		// 	set_tacho_command_inx( motor, TACHO_RUN_TO_REL_POS );
		// 	Sleep( 500 );
		// }
	} else {
		printf( "LEGO_EV3_M_MOTOR 1 is NOT found\n" );
	}
}

int main(int argc, char **argv)
{
    sleep(2);
    #ifndef __ARM_ARCH_4T__
    /* Disable auto-detection of the brick (you have to set the correct address below) */
    ev3_brick_addr = "192.168.0.204";
    #endif
    if ( ev3_init() == -1 ) return ( 1 );
    #ifndef __ARM_ARCH_4T__
    printf( "The EV3 brick auto-detection is DISABLED,\nwaiting %s online with plugged tacho...\n", ev3_brick_addr );
    #else
    printf( "Waiting tacho is plugged...\n" );
    #endif
    while ( ev3_tacho_init() < 1 ) Sleep( 1000 );
    ev3_sensor_init();
    printf( "*** ( EV3 ) Hello! ***\n" );
    printf( "Found tacho motors:\n" );
    //setGrabbingMotor();
    release();
    return 0;
}
