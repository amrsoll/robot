#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "move.h"
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"

#define Sleep( msec ) usleep(( msec ) * 1000 )

int position;

uint8_t sn1;
uint8_t sn2;
uint8_t sn3;
//define all sensors
uint8_t sn_sonar;

int max_speed;

float sonar_abs;
float sonar_val;
float value = 0;
float distance = 200.0;
int motorports[2] = {67,68};

int init_position;

int main(int argc, char **argv) {
    init();
    uint8_t motor_left  = ev3_search_tacho_plugged_in(motorports[0],0, &sn, 0 );
    uint8_t motor_right = ev3_search_tacho_plugged_in(motorports[1],0, &sn, 0 );
    uint8_t motors[2] = {motor_left, motor_right};
    move(sn_sonar, motors, distance);
    return 0;
}

int init(){
    #ifndef __ARM_ARCH_4T__
    /* Disable auto-detection of the brick (you have to set the correct address below) */
    ev3_brick_addr = "192.168.0.204"

    #endif
    if ( ev3_init() == -1 ) return ( 1 );

    #ifndef __ARM_ARCH_4T__
    printf( "The EV3 brick auto-detection is DISABLED,\n waiting %s online with plugged tacho...\n", ev3_brick_addr );
    #else
    printf( "Waiting tacho is plugged...\n" );
    #endif

    while ( ev3_tacho_init() < 1 ) Sleep( 1000 );
    printf( "*** ( EV3 ) Hello! ***\n" );
    printf( "Found tacho motors:\n" );

    //Check for motors
    if (ev3_search_tacho_plugged_in(motorports[1],0,&sn1,0) == 0){
        printf("The right engine is not plugged, please plug it")
    }
    if (ev3_search_tacho_plugged_in(motorports[0],0,&sn2,0) == 0){
        printf("The left engine is not plugged, please plug it")
        //here add one for the piliers
    }
    set_tacho_command_inx(sn1, TACHO_STOP);
    set_tacho_command_inx(sn2, TACHO_STOP);
    // ADD pliers
    /*   int port=65;
    for (port=65; port<69; port++){
    if ( ev3_search_tacho_plugged_in(port,0, &sn, 0 )
    */
    if (ev3_search_tacho_plugged_in(motorports[0],0,&sn1,0)){
        printf( "LEGO_EV3_M_MOTOR 1 is found, run for 5 sec...\n" );

        get_tacho_max_speed( sn1, &max_speed );
        printf("  max speed = %d\n", max_speed );

        set_tacho_stop_action_inx( sn1, TACHO_COAST );
        set_tacho_speed_sp( sn1, max_speed * 2 / 3 );
        set_tacho_time_sp( sn1, 1000 );
        set_tacho_ramp_up_sp( sn1, 0 );
        set_tacho_ramp_down_sp( sn1, 0 );
    }  else {
        printf( "LEGO_EV3_M_MOTOR 1 is NOT found\n" );
    }
    if (ev3_search_tacho_plugged_in(motorports[1],0,&sn2,0){
        printf( "LEGO_EV3_M_MOTOR 2 is found, run for 5 sec...\n" );
        get_tacho_max_speed( sn2, &max_speed );
        printf("  max speed = %d\n", max_speed );

        set_tacho_stop_action_inx( sn2, TACHO_COAST );
        set_tacho_speed_sp( sn2, max_speed * 2 / 3 );
        set_tacho_time_sp( sn2, 1000 );
        set_tacho_ramp_up_sp( sn2, 0 );
        set_tacho_ramp_down_sp( sn2, 0 );
    }  else {
        printf( "LEGO_EV3_M_MOTOR 2 is NOT found\n" );
    }

    //Check the sensors
    ev3_sensor_init();
    if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0) == 0){
        printf("SONAR missing\n");
    }
    get_tacho_position(sn1, &init_pos);
    set_tacho_command_inx( sn1, TACHO_STOP );
    set_tacho_command_inx( sn2, TACHO_STOP );

    Sleep(400);
    return 0;
}
