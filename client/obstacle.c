#include "obstacle.h"


int detect_obstacle(uint8_t sn_color, uint8_t sn_sonar, int threshold) {
    int detected_color;
    float distance;

    distance = get_sonar_value(sn_sonar);

    while(distance > threshold) {
        //move a little closer
       
    } 
    detected_color = (int)get_color_value(sn_color);
            
    if(detected_color == 5) {
        printf("[OBSTACLE] Moveable obstacle detected!\n");
        return 1;
    } else {
        //back up
        printf("[OBSTACLE] Non-moveable obstacle detected!/n");
        return 0;
    }
}
