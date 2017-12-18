#include "obstacle.h"

int eval_obstacle() {
    bool left, right;

    float distance_front, distance_left, distance_right;

    get_sensor_value0(sn_gyr, &distance_front);  //should be the threshold value, 300

    //check left
    turn_absolute(motor_right, max_speed, 1, 30.0);
    Sleep(500);
    get_sensor_value0(sn_gyr, &distance_left);
    //do some trigonometry here
    if(distance_left > trig_result) {
        left = false;
    } else { left = true;}

    //check right
    turn_absolute(motor_left, max_speed, 1, 60.0);
    Sleep(1000);
    get_sensor_value0(sn_gyr, &distance_right);
    //trigonometry here as well
    if(distance_right > trig_result) {
        right = false;
    } else { right = true;}

    if( left && right) {
        printf("[OBSTACLE] Moveable obstacle detected!\n");
    } else if(left || right) {
        printf("[OBSTACLE] Maybe, maybe not\n");
    } else { 
        printf("[OBSTACLE] Non-moveable obstacle detected!\n"); 
    }


    //turn back to initial direction
    turn_absolute(motor_right, max_speed, 1, 30.0);
    Sleep(500);


}
