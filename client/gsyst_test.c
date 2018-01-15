/**
 * @Author: Axel_Soll <madafaka>
 * @Date:   15/01/2018
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   madafaka
 * @Last modified time: 15/01/2018
 */

#include "gsyst.h"

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

void main()
{
    sensor_init();
    init_mov_motors();
    float measured_angle, value;
    turn_to_angle(45);
    // while(1)
    // {
    // 	if ( !get_sensor_value0(sn_compass, &value )) {
    //      	value = 0;
    // 	}
    //
    // 	printf( "\r compass : %f \n", value);
    //
    //     get_sensor_value0(sn_gyr,&measured_angle);
    // 	printf( "\r gyro :    %f \n", measured_angle);
    //     sleep(1);
    //
    // }
}
