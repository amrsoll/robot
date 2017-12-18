#include "ball.h"

const char const *colourName[] = { "?", "BLACK", "BLUE", "GREEN", "YELLOW", "RED", "WHITE", "BROWN" };
#define NB_COLOUR  (( int )( sizeof( colourName ) / sizeof( colourName[ 0 ])))

int init_colour()
{
    sn_col = sensor_search( LEGO_EV3_COLOR );
    if (sn_col)
    {
        if(color_set_mode_col_color(sn_col))
        {
            printf("failed to change the mode of the color sensor. \n");
            return -1;
        } else {
            printf( "colour sensor connected.\n");
        }
    }
    return 0;
}

int detect_obstacle()
{
}

int detect_movable()
//rides up to the obstacle and detects if it has a red colour or not. Returns 1 if not, returns 0 if it is red.
{
    float dist;
    init_colour();
    ev3_search_sensor( LEGO_EV3_US, &sn_sonar ,0)
    get_sensor_value0(sn_sonar, &dist );
    if(dist < (float)(DISTANCE_DETECT_MOVABLE - ULTRASONIC_SENSOR_PRECISION))
    {
        continue_until(-max_speed,(float)(DISTANCE_DETECT_MOVABLE));
    } else if(dist > (float)(DISTANCE_DETECT_MOVABLE + ULTRASONIC_SENSOR_PRECISION))
    {
        continue_until(max_speed,(float)(DISTANCE_DETECT_MOVABLE));
    }
    sleep(500);
    colour=sensor_get_value0(sn_col,0);
    if(colour > NB_COLOUR)
    {
        printf("Unknown colour. Colour number : %d\n", colour);
        return 1;
    } else if(colourName[colour]=="RED")
    {
        printf("movable obstacle detected  !!  \n");
        return 0;
    } else {
        return 1;
    }
}
