/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   13/01/2018
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   amrsoll
 * @Last modified time: 15/01/2018
 */



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> //for use of the random function

#include "constants.m"

#include "gsyst.h"
#include "map.h"
#include "path.h"
#include "spot.h"
#include "servercom.h"

volatile int DONE_EXPLORING = 0;
int16_t posX, posY; //positions sent to the server
int s; //socket

int sensor_init() {
    ev3_sensor_init();
    if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0) == 0){
        printf("SONAR missing\n");
        return -1;
    }
    if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_gyr,0) == 0){
        printf("Gyro missing\n");
        return -1;
    }
    get_sensor_value0(sn_gyr,&init_angle);
    refresh_angle();
    refresh_distance();
    init_distance = distance;
    return 0;
}

int motors_init() {
    #ifndef __ARM_ARCH_4T__
    // Disable auto-detection of the brick
    //(you have to set the correct address below)
    ev3_brick_addr = EV3_BRICK_ADDR;
    #endif
    if ( ev3_init() == -1 ) return ( 1 );
    while ( ev3_tacho_init() < 1 ) Sleep( 1000 );
    printf( "Found tacho motors:\n" );
    return 0;
}

int setPosition() {
    posX = (int16_t)(robotPosition.x*MM_TO_PIX_SIZE_TO_SERVER);
    posY = (int16_t)(robotPosition.y*MM_TO_PIX_SIZE_TO_SERVER);
}

void *thSendPosition() {
    while(!DONE_EXPLORING) {
        Sleep(2000);
        setPosition();
        send_POSITION(s, posX, posY);
    }
    pthread_exit(NULL);
}

void *thReceiveFromServer() {
    while(!DONE_EXPLORING) {
        parse_message(s);
    }
    pthread_exit(NULL);
}

#define LARGE_ARENA
////////////////////////////////////////////////////////////////////////////////
#ifdef LARGE_ARENA

int main(int argc, char **argv) {
    if(argc!=3)
    {
        printf("you must precise the width and height of the map : \n");
        printf("./servercom <width> <height>\n");
    }
    int width = atoi(argv[1]); //DOES NOT INCLUDE THE \n at the right edge of the mapstring
    int height = atoi(argv[2]);


    sensor_init();
    motors_init();

    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    /* threads */
    pthread_t positioning;
    pthread_t receiving;

    /* SET UP BT CONNECTION TO SERVER */
    struct sockaddr_rc addr = { 0 };
    int status;

    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba(SERV_ADDR, &addr.rc_bdaddr);

    /* connect */
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    printf("status: %d\n", status);

    if(status!=0 || getStartSignal()) {  //counterintuitive : in C, the while loop continues as long as it is given an int !=0
        exit(EXIT_FAILURE); //if we fail to connect or get the startsignal

    // initialise the position of the robot
    robotPosition = fPoint_new(.0,.0);

    //fork a process that will ping the server every 2 sec with the position of the robot
    if(pthread_create(&positioning, NULL, thSendPosition, NULL)) {
        fprintf(stderr, "Error creating thread\n");
        exit(EXIT_FAILURE);
    }
    if(pthread_create(&receiving, NULL, thReceiveFromServer, NULL)) {
        fprintf(stderr, "Error creating thread\n");
        exit(EXIT_FAILURE);
    }
    printf("Threads created successfully\n");

    const char* map = get_new_local_map(width, height);
    start_position = tCoord_new(width/2, height -300*MM_TO_PIX_SIZE);


    // scan a first time to get the surroundings :
    scan(robotPosition, start_position, width, height, map);
    tCoord previousSpots[4*width*height/(DIST_MIN_FROM_WALLS*DIST_MIN_FROM_WALLS)];
    size_t size_previousSpots = 0;
    while(!mapComplete(map))
    {
        tCoord spot = getNewSpot(); //reads & writes into path file.
        if(getPathTo(spot))
        {
            printf("failed to find the path to the new coordinates");
            exit(EXIT_FAILURE);
        }

        int pathLen = countlines(path);
        int i;
        for(i=0; i<pathLen; i++)
        {
            tCoord checkPoint = getCheckpoint(i, pathLen);
            if(moveTo(checkPoint, start_position, map)) //error prone
            {
                printf("failed to move to the next coordinates");
                exit(EXIT_FAILURE);}
            }
            if(i>1)
            {
                setPosition();
                send_POSITION(s, posX, posY); //send position when the robot stops
            }
        }
        scan(robotPosition, start_position, width, height, map);
    }
    DONE_EXPLORING = 1;
    sendMap();
}
#endif
////////////////////////////////////////////////////////////////////////////////
#ifdef SMALL_ARENA

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
    init_mov_motors();

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
        continue_until(DISTANCE_BEFORE_STOP);

        printf("we found a wall, now turning\n" );
        turn_absolute(mov_motors[rand()%2],1,90.0);
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
}
#endif
