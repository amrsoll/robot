#include "obstacle.h"

/* this function assumes the obstacle is hit at 90 degrees... */
int eval_obstacle() {
    bool left, right;

    float distance_front, distance_left, distance_right;

    get_sensor_value0(sn_sonar, &distance_front);  //should be the threshold value, 300

    //check left
    turn_absolute(mov_motors[1], max_speed, 1, TURN_DEGREE);
    Sleep(500);
    get_sensor_value0(sn_sonar, &distance_left);
    if(distance_left > CHECK_DISTANCE) {
        left = false;
    } else { left = true;}

    //check right
    turn_absolute(mov_motors[0], max_speed, 1, 2*TURN_DEGREE);
    Sleep(1000);
    get_sensor_value0(sn_sonar, &distance_right);
    if(distance_right > CHECK_DISTANCE) {
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
    turn_absolute(motor_right, max_speed, 1, TURN_DEGREE);
    Sleep(500);


}
