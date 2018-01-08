#include <stdio.h>
#include <stdlib.h>
#include "gsyst.h" /* Also contains the ev3 libraries*/
#include "map.h"


#define SCAN_TEST

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
    float init_angle = get_angle();
    float angle = init_angle;
    float x = 0.0;
    float y = 0.0;init();
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


#ifdef SCAN_TEST
    init_mov_motors();
    char* scanResult = scan();
    printf("%s\n",scanResult );
    free(scanResult); //prevent memory leaks
#endif

}
