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
    init();
    init_angle = get_angle();
    refresh_angle();
    x = 0.0;
    y = 0.0;
    int refreshdist = 0;
    while(refreshdist++<20) refresh_distance();
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
    tCoord coord = tCoord_new(2,5);
    set_char(coord,10,3,'a',str);
    printf("%s\n", str);
    printf("sin pi %f\n", sin(pi));
    printf("sin 90 %f\n", sin(90));
    printf("sin 45 %f\n", sin(45));
    printf("sin pi/2 %f\n",sin(pi/2));
#endif
#ifdef INTERSECT_TEST
    int i = 1;
    int width = 20;
    int height = 20;
    char* tryout = get_new_local_map(width, height);
    width++;
    Point p1 = Point_new(5,15);
    Point p2 = Point_new(15,5);
    Point O = Point_new(2,2);
    Point s = Point_new(min(min(p1.x, p2.x),O.x),
                        min(min(p1.y, p2.y),O.y));
    int sy=s.y;
    tCoord origin = tCoord_new(height-1,0);

    while (s.x< max(max(p1.x, p2.x),0))
    {
        while (s.y< max(max(p1.y, p2.y),0))
        {
            tCoord ts = Point_to_tCoord(s,origin);
            if( i==0 ) {
                if(intpoint_in_trigon(s, O, p1, p2))
                    set_char(ts,width,height,FREE_PIXEL,tryout);
            }
            else{
                if(intsquare_fray_intersect(s,Point_to_fPoint(O),Point_to_fPoint(p1)))
                    set_char(ts,width,height,FREE_PIXEL,tryout);
            }
            //set_char(ts,width,height,FREE_PIXEL,tryout);
            s.y++;
        }
        s.y=sy;
        s.x++;
    }
    set_char(Point_to_tCoord(p1,origin),width,height,'A',tryout);
    set_char(Point_to_tCoord(p2,origin),width,height,'B',tryout);
    set_char(Point_to_tCoord(O,origin),width,height,'O',tryout);

    printf("%s\n",tryout);
#endif
#ifdef GYRO_TEST
    init();
    init_angle = get_angle();
    refresh_angle();
    x = 0.0;
    y = 0.0;
    int refreshdist = 0;
    while(refreshdist++<20) refresh_distance();
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
    float ang;
    while(1)
    {
        refresh_angle();
        printf("angle : %f\n", angle);
    }
#endif
}
