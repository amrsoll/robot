#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ev3.h"
#include "move.h"
#include "turn_back.h"

//#include "turn_function_left.h"
//#include "turn_function_right.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"

#define Sleep( msec ) usleep(( msec ) * 1000 )




//define position of robot
int16_t x_axe;
int16_t y_axe;
int stop;
int moving_step;
int position;

unsigned char rank = 0;
unsigned char lenght = 0;
unsigned char previous = 0xFF;
unsigned char next = 0xFF;

int s;

//DEfinition of variables


uint8_t sn1;
uint8_t sn2;
uint8_t sn3;
FLAGS_T state =1;
int max_speed;
float sonar_abs;
float sonar_val;
float value = 0;
float init_sonar;
float value_sonar;
int init_position;
int case_pos=0;
int save_position;

uint8_t motor[2]={DESC_LIMIT,DESC_LIMIT};

//define all sensors
uint8_t sn_sonar;

//we will init all sensors, motors, and check if it's plugged or not.


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

    if (ev3_search_tacho_plugged_in(65,0,&sn1,0) == 0){
        printf("The right engine is not plugged, please plug it");
    }
    if (ev3_search_tacho_plugged_in(66,0,&sn2,0) == 0){
        printf("The left engine is not plugged, please plug it");
        //here add one for the piliers
    }



    set_tacho_command_inx(sn1, TACHO_STOP);
    set_tacho_command_inx(sn2, TACHO_STOP);
    // ADD pliers

    /*   int port=65;
    for (port=65; port<69; port++){
    if ( ev3_search_tacho_plugged_in(port,0, &sn, 0 )
    */


    if (ev3_search_tacho_plugged_in(65,0,&sn1,0)){


        printf( "LEGO_EV3_M_MOTOR 1 is found, run for 5 sec...\n" );

        get_tacho_max_speed( sn1, &max_speed );
        printf("  max speed = %d\n", max_speed );

        set_tacho_stop_action_inx( sn1, TACHO_COAST );
        set_tacho_speed_sp( sn1, max_speed * 3 );
        set_tacho_time_sp( sn1, 5000 );
        set_tacho_ramp_up_sp( sn1, 0 );
        set_tacho_ramp_down_sp( sn1, 0 );
    }  else {
        printf( "LEGO_EV3_M_MOTOR 1 is NOT found\n" );
    }


    if (ev3_search_tacho_plugged_in(66,0,&sn2,0)){

        printf( "LEGO_EV3_M_MOTOR 2 is found, run for 5 sec...\n" );


        get_tacho_max_speed( sn2, &max_speed );
        printf("  max speed = %d\n", max_speed );

        set_tacho_stop_action_inx( sn2, TACHO_COAST );
        set_tacho_speed_sp( sn2, max_speed *  3 );
        set_tacho_time_sp( sn2, 5000 );
        set_tacho_ramp_up_sp( sn2, 0 );
        set_tacho_ramp_down_sp( sn2, 0 );

    }  else {
        printf( "LEGO_EV3_M_MOTOR 2 is NOT found\n" );
    }


    ev3_sensor_init();

    if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_sonar,0) == 0){
        printf("SONAR missing\n");
        while (!get_sensor_value0(sn_sonar, &init_sonar)){
        }
    }

    get_tacho_position(sn1, &init_position);

    set_tacho_command_inx( sn1, TACHO_STOP );
    set_tacho_command_inx( sn2, TACHO_STOP );

    motor[0]=sn1;
    motor[1]=sn2;

    Sleep(4000);

    return 0;
}

/*
void* beginner(){
char string[58];
printf("I'am the beginner...");

int temp=1;
if (position==2){
while (temp!=0){
pthread_mutex_lock(&mutex_moving);
pthread_cond_wait(&condition_beginner,&mutex_moving);
moving=0;
printf("I'm no longer waiting\n");
pthread_mutex_unlock (&mutex_moving);
break;
}
}

if (position==1){
//The beginner is in position 1 and has to go to position 2, and then send a next message

//set the initial position of the robot

pthread_mutex_lock(&mutex_x_robot);
x_robot=90;
pthread_mutex_unlock(&mutex_x_robot);

pthread_mutex_lock(&mutex_y_robot);
y_robot=20;
pthread_mutex_unlock(&mutex_y_robot);

//the robot is moving so needs to send its position
pthread_mutex_lock(&mutex_moving);
moving=0;
pthread_mutex_unlock (&mutex_moving);

//From 1 to the middle to drop the ball

case_pos = 0;
//bouger_sonar(sn1, sn2, sn_gyro, sn_sonar, motor, value, gyro_val, gyro_abs, state, 930);
case_pos = 6;
turn_fuction_left(sn1, sn2, sn_gyro, motor, value, gyro_val, gyro_abs, state);
case_pos = 3;
//bouger_sonar(sn1, sn2, sn_gyro, sn_sonar, motor, value, gyro_val, gyro_abs, state, 436);
case_pos = 6;
turn_back(sn1, sn2, sn_gyro, motor, value, gyro_val, gyro_abs, state);
move(sn1, sn2, sn_sonar, motor, value, sonar_val, sonar_abs,10);

//drop the ball or drop it and send the message it was done
//poser_balle(sn3);

*((uint16_t *) string) = msgId++;
string[2] = TEAM_ID;
string[3] = next;
string[4] = MSG_BALL;
string[5]=0x0;
string[6] = x_robot;          // x
string[7] = 0x00;
string[8] = y_robot;		//y
string[9]= 0x00;
write(s, string, 10);

//From the middle where the ball was to the position 2 (top right)
case_pos = 4;
go_back(sn1, sn2, sn_gyro, motor, value, gyro_val, gyro_abs, state, 250);
case_pos = 6;
turn_back(sn1, sn2, sn_gyro, motor, value, gyro_val, gyro_abs, state);
case_pos = 3;
///bouger_sonar(sn1, sn2, sn_gyro, sn_sonar, motor, value, gyro_val, gyro_abs, state, 160);
case_pos = 6;
turn_function_left(sn1, sn2, sn_gyro, motor, value, gyro_val, gyro_abs, state);
case_pos = 1;
//bouger_sonar(sn1, sn2, sn_gyro, sn_sonar, motor, value, gyro_val, gyro_abs, state, 230);

//Preemptive motors stop
set_tacho_command_inx( sn1, TACHO_STOP );
set_tacho_command_inx( sn2, TACHO_STOP );
ev3_uninit();

//the robot has now to stop sending messages of its position
pthread_mutex_lock(&mutex_moving);
moving=1;
pthread_mutex_unlock (&mutex_moving);

//robot is now in position 2
position=2;

//send the next message
Sleep(3000);
*((uint16_t *) string) = msgId++;
string[2] = TEAM_ID;
string[3] = next;
string[4] = MSG_NEXT;
write(s, string, 5);
}

else if (position==2){ //the robot has to go from 2 to 1, and then from 1 to 2 without sending a message

pthread_mutex_lock(&mutex_moving);
moving=0;
pthread_mutex_unlock (&mutex_moving);

//From 2 to the zone where the ball is
bouger_sonar(sn1, sn2, sn_gyro, sn_sonar, motor, value, gyro_val, gyro_abs, state, 750);
case_pos = 6;
turn_fuction_right(sn1, sn2, sn_gyro, motor, value, gyro_val, gyro_abs, state);
case_pos = 3;
bouger_sonar(sn1, sn2, sn_gyro, sn_sonar, motor, value, gyro_val, gyro_abs, state, 510);
go_back(sn1, sn2, sn_gyro, motor, value, gyro_val, gyro_abs, state, 300);
case_pos = 6;

//search for the ball
grab_droite(sn1, sn2, sn3, sn_gyro, sn_sonar, motor, value, gyro_val, gyro_abs, state);

//pick the ball and send the message it was done
*((uint16_t *) string) = msgId++;
string[2] = TEAM_ID;
string[3] = next;
string[4] = MSG_BALL;
string[5]=0x1; //pick the ba ll
string[6] = x_robot;          // x
string[7] = 0x00;
string[8] = y_robot;		//y
string[9]= 0x00;
write(s, string, 10);

//from the zone where the ball was to position 1
case_pos = 3;
//bouger_sonar(sn1, sn2, sn_gyro, sn_sonar, motor, value, gyro_val, gyro_abs, state, 230);
case_pos = 6;
turn_function_right(sn1, sn2, sn_gyro, motor, value, gyro_val, gyro_abs, state);
case_pos = 0;
//bouger_sonar(sn1, sn2, sn_gyro, sn_sonar, motor, value, gyro_val, gyro_abs, state, 230);

//Preemptive motors stop
set_tacho_command_inx( sn1, TACHO_STOP );
set_tacho_command_inx( sn2, TACHO_STOP );
ev3_uninit();

//we are now in position 1 and need to go back to 2
position=1;
}

}


*/

int main() {
    init();
	
    move(sn_sonar,10, 10,  motor, value, sonar_val, sonar_abs,state, 1000);
//turn_back(sn_sonar,6,6 ,  motor, value, sonar_val, sonar_abs,22);
    return 0;
}