#include "sensors.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include "ev3.h"
//#include "ev3_sensor.h"


const char const *color[] = { "?", "BLACK", "BLUE", "GREEN", "YELLOW", "RED", "WHITE", "BROWN" };
#define COLOR_COUNT (( int )( sizeof( color ) / sizeof( color[ 0 ])))


void sensor_init(uint8_t *sensorsIndex) {

    // these should maybe be defined in main
    // and be brought in as pointers?
    uint8_t sn_color;
    uint8_t sn_sonar;
    uint8_t sn_gyro;

    ev3_sensor_init();
    
    if(ev3_search_sensor( LEGO_EV3_COLOR, &sn_color, 0)) {
       sensorsIndex[2] = sn_color;
    } else printf("Color sensor (port 3) not attached");

    if(ev3_search_sensor( LEGO_EV3_US, &sn_sonar, 0)) {
        sensorsIndex[0] = sn_sonar;
    } else printf("Sonar sensor (port 1) not attached");

    if(ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyro, 0)) {
        sensorsIndex[1] = sn_gyro;
    } else printf("Gyro sensor (port 2) not attached");
        
    set_sensor_mode(sensorsIndex[1], "COL-COLOR");
}

/*
float get_compass_value(uint8_t sn_compass) {
    int i;
    int measurements;
    float sum;
    float value;

    measurements = 50;
    sum = 0;

    // calculate average
    for(i = 0; i < measurements; i++) {
        if(!get_sensor_value0(sn_compass, &value)) {
            value = 0;
        }

        Sleep(100);
        sum += value;

    }
    return sum/measurements;

}
*/

float get_sonar_value(uint8_t sn_sonar) {
    int i;
    int measurements,
    float value;
    float sum;

    measurements = 5;
    sum =0;
    
    // calculate average
    for(i = 0; i < measurements; i++) {
        if(!get_sensor_value(sn_sonar, &value)) {
            value = 0;
        }
        Sleep(50);
        sum += value;
    }

    return sum/measurements;
   
}

int get_color_value(uint8_t sn_color) {
    int val;

    if(!get_sonar_value(0, sn_color, &val) || (val < 0) || (val >= COLOR_COUNT)) val = 0;

    return val;
}

float get_gyro_value(uint8_t sn_gyro) {
    float value;
    if ( !get_sensor_value0(sn_gyro, &value )) {
                    value = 0;
    }
    return value;
}


