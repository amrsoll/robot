/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   08/01/2018
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   amrsoll
 * @Last modified time: 09/01/2018
 */


#define SCAN_TEST

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "gsyst.h" /* Also contains the ev3 libraries*/
#include "map.h"


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

void sigint_handler()
{
    stop_mov_motors();
    exit(1);
}

int main(int argc, char **argv) {


#ifdef SCAN_TEST
    float init_angle = get_angle();
    angle = get_angle() - init_angle;
    float x = 0.0;
    float y = 0.0;
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
    signal(SIGINT, sigint_handler);
    init_mov_motors();
    char* scanResult = scan();
    printf("%s\n",scanResult );
    free(scanResult); //prevent memory leaks
#endif
#ifdef STR_TEST
    char str[29] = "123456789\n123456789\n123456789";
    set_char(2,5,10,3,'a',str);
    printf("%s\n", str);
#endif

}
