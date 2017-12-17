#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "constants.h"

#include "sensors.h"
#include "gsyst.h"
#include "map.h"
#include "path.h"
#include "spot.h"
#include "servercom.h"

#define Sleep( msec ) usleep(( msec ) * 1000 )
float error=0,correction=0;
float value;
float result;
int maxmedium;
float initial_ang,min_dist;
uint8_t sn[2];
FLAGS_T state;
float value;
uint8_t sn_sonar;
uint8_t sn_gyr;
int max_speed;
int maxmedium;
int i;
char s[ 256 ];
int init() {
    ev3_sensor_init();
    if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0) == 0){
        printf("SONAR missing\n");
        while (!get_sensor_value0(sn_sonar, &value)){
        }
    }
    if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_gyr,0) == 0){
        printf("Gyro missing\n");
        while (!get_sensor_value0(sn_sonar, &value)){
        }
        return 0;
    }
}


int main(int argc, char **argv) {
    init();
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
    for ( i = 0; i < DESC_LIMIT; i++ )
    {
        if ( ev3_tacho[ i ].type_inx != TACHO_TYPE__NONE_ )
        {
            printf( "  type = %s\n", ev3_tacho_type( ev3_tacho[ i ].type_inx ));
            printf( "  port = %s\n", ev3_tacho_port_name( i, s ));
            printf("  port = %d %d\n", ev3_tacho_desc_port(i), ev3_tacho_desc_extport(i));
        }
    }

    //Run motors in order from port A to D
    int port1=65,port2=66,port3=68;
    uint8_t s1,s2;
    if ( ev3_search_tacho_plugged_in(port1,0, &s1, 0 ) && ev3_search_tacho_plugged_in(port2,0, &s2, 0 ))
    {
        int max_speed;
        sn[0]=s1;
        sn[1]=s2;
        if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
        {
            get_sensor_value0(sn_gyr,&initial_ang);
            printf( "  value initial angle: %f\n",initial_ang);
        }
        fflush( stdout );
        multi_set_tacho_command_inx( sn, TACHO_RESET );
        printf( "2 LEGO_EV3_M_MOTORs  are found, run for 5 sec...\n" );
        get_tacho_max_speed( sn[0], &max_speed );
        printf("  max speed = %d\n", max_speed );
        //FIRST PROGRAM
        //********go near first obstacle****************
        Sleep(2000);
        continue_until(max_speed,300);
        Sleep(1000);
        //********first turn to the right****************
        //    turn_absolute(s1,max_speed,1,90.0);
        //    Sleep(1000);
        float dist;
        get_sensor_value0(sn_sonar, &dist );
        printf("Cas demande %f\n",dist);
        while(1)
        {
            if(dist<300)
            {
                printf("Cas demande\n" );
                get_sensor_value0(sn_sonar, &dist );
                printf("Cas demande %f\n",dist);
                turn_absolute(s2,max_speed,1,90.0);
                Sleep(1000);
            }
            continue_until(max_speed,300);
            printf(" NATTTTT %f\n",dist);
        }

    }
}
