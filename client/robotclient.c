#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> //for random

#include "constants.m"

#include "gsyst.h"
//#include "map.h"
#include "path.h"
#include "spot.h"
#include "servercom.h"



int init() {
    ev3_sensor_init();
    if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0) == 0){
        printf("SONAR missing\n");
        return -1;
    }
    if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_gyr,0) == 0){
        printf("Gyro missing\n");
        return -1;
    }
    return 0;
}

int main(int argc, char **argv) {
    init();
    #ifndef __ARM_ARCH_4T__
        // Disable auto-detection of the brick
        //(you have to set the correct address below)
        ev3_brick_addr = EV3_BRICK_ADDR;
    #endif
    if ( ev3_init() == -1 ) return ( 1 );
    #ifndef __ARM_ARCH_4T__
        printf("The EV3 brick auto-detection is DISABLED,\n");
        printf("waiting %s online with plugged tacho...\n", ev3_brick_addr );
    #else
        printf( "Waiting tacho is plugged...\n" );
    #endif

    while ( ev3_tacho_init() < 1 ) Sleep( 1000 );
    printf( "*** ( EV3 ) Hello! ***\n" );
    printf( "Found tacho motors:\n" );
    
    //Find the Gyro and get the initial angle from it
    init_angle = get_angle();
    printf("Initial angle value: %f\n",init_angle);
    pincer_state = PINCER_OPENED;  //Change according to the starting position of the pincer
    if(pincer_state == PINCER_OPENED)
    {
        printf("grabbing, pincer state before : %d\n", pincer_state);
        grab(-1);
        // do
        //     get_tacho_state_flags(grab_motor, &state );
        // while ( state );
        pincer_state = PINCER_CLOSED;
        printf("grabbing, pincer state after  : %d\n", pincer_state);
    }

    //Get the max speed the moving motors can get to
    init_mov_motors();
    int max_speed;
    //fflush( stdout );
    //multi_set_tacho_command_inx( mov_motors, TACHO_RESET );
    //printf( "2 LEGO_EV3_M_MOTORs  are found, run for 5 sec...\n" );
    get_tacho_max_speed( mov_motors[0], &max_speed );
    //printf("  max speed = %d\n", max_speed );

    //FIRST PROGRAM
    //Initialise the positioning values
    int continue_while = 1; // Continue demo until this changes to 0.
    x = 0.0;
    y = 0.0;
    angle = 0.0;
    float dist;
    int number_turns = 0;
    get_sensor_value0(sn_sonar, &dist );
    while(continue_while)
    {
        continue_until(max_speed,DISTANCE_BEFORE_STOP);

        printf("we found a wall, now turning\n" );
        turn_absolute(mov_motors[rand()%2],max_speed,1,90.0);
        // do
        //     get_tacho_state_flags(mov_motors[0], &state );
        // while ( state );
        printf(" NATTTTT %f\n",dist);
        if(number_turns>3 && pincer_state == PINCER_CLOSED)
        {
            printf("releasing\n");
            grab(1);
            // do
            //     get_tacho_state_flags(grab_motor, &state );
            // while ( state );
            pincer_state = PINCER_OPENED;
        }
        number_turns = number_turns+1;
    }

    /*
    startBt();
    getStartSignal(conn); //blocking function. Does not continue without getting signal
    int x = 0;
    int y = 0;
    //fork a process that will ping the server every 2 sec with the position of the robot
    while(!mapComplete()) //counterintuitive : in C, the while loop continues as long as it is given an int !=0
    {
    sendPosition(x, y);
    if(map()){
    printf("failed to map the surroundings");
    exit(EXIT_FAILURE);}
    int* spot = getNewSpot();
    if(getPathTo(*spot[0],*spot[1])) {
    printf("failed to find the path to the new coordinates");
    exit(EXIT_FAILURE);}
    FILE *path = open("~/path");
    int pathLen = countlines(path);
    for(int i=0; i<pathLen; i++)
    {
    int k = n-i;
    nextNode = node_init_str(readline(*path, k));
    if(!(x,y = moveTo(x, y, nextNode.x, nextNode.y)){ //error prone
    printf("failed to move to the next coordinates");
    exit(EXIT_FAILURE);}
    if(k>0)
    sendPosition(x, y);
}
}
sendMap();
*/
}
