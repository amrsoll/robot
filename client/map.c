#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include "gsyst.h" /* Also contains the ev3 libraries*/
#include "classes.m"
#include "constants.m"

#include "map.h"

#define max(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })
#define min(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })

int set_char(int i, int j, char value, char* str) //Changes the char value str on line i and j
{
    char c;
    while(i>0)
    {
        while(c!='\n') c = str++[0];
        i--;
    }
    str[j] = value;
    return 0;
}

char get_char(int i, int j, char* str)
//i0 and j0 is the position of the origin of the x and y axis
{
    char c;
    while(i>0)
    {
        while(c!='\n') c = str++[0];
        i--;
    }
    return str[j];
}

void free_isolated_cells(char* map)
//if a pixel is directly surrounded by four pixels of a different type,
//then turn it into that type
{
    // TODO
    // be careful with borders
}

char* scan() //returns the string result of the scan
{
    float step_fill_free_space = 0.2;
    int mm_to_pixel_size = 10*PIXEL_SIZE;
    char* scanResult = NULL;
    int width  = 2*SCANNING_MAX_DISTANCE/mm_to_pixel_size+2; //adding one in case of debordement
    int height = 2*SCANNING_MAX_DISTANCE/mm_to_pixel_size+2;
    scanResult = malloc((width+1)* //because we add \n at each line
                        height*
                        sizeof(char));
    if (scanResult == NULL) exit(-1); // Error : failed to allocate memory
    int robot_coord_in_scanResult_str[2] = {width/2,height/2};
    float starting_scan_angle = angle; //angle is created in contants and constantly refreshed
    // float* buffer = NULL;
    // buffer = malloc(10*sizeof(int)/SCANNING_SPEED);
    // if (buffer == NULL) exit(-1); // Error : failed to allocate memory
    // unsigned short int buffer_index = 0;
    Point last_point; //so as to not constantly change the same pixel
    last_point.x = 0;
    Point measured_point; //coordinates of the tile/pixel the sonar hits into.
    int max_speed;
    get_tacho_max_speed( mov_motors[0], &max_speed ); //temporary, wait for revision of gsyst
    printf("max speed : %d\n", max_speed);
    start_turn(max_speed, SCANNING_SPEED);
    float distance;
    while(abs(starting_scan_angle-angle)<360)
    {
        angle = get_angle() - init_angle;
        distance = min(SCANNING_MAX_DISTANCE, get_distance());
        fPoint fmeasured_point = {(distance*cos(angle/pi)/mm_to_pixel_size),
                                (distance*sin(angle/pi)/mm_to_pixel_size)};
        Point measured_point;
        measured_point.x =(int)fmeasured_point.x;
        measured_point.y =(int)fmeasured_point.y;
        if( last_point.x == 0
            || point_eq(last_point, measured_point))
        {
            last_point = measured_point;
            set_char(robot_coord_in_scanResult_str[0]-measured_point.y,
                     robot_coord_in_scanResult_str[1]+measured_point.x,
                     WALL_PIXEL,
                     scanResult);
            //fill the pixels between the wall and you with free space
            //if it's not already a wall or free space
            if( last_point.x==0 )
            {
                int len_steps = (int)distance/mm_to_pixel_size;
                Point coord_free;
                Point last_coord_free;
                last_coord_free.x = 0;
                int i=0;
                while(i<len_steps)
                {
                    coord_free.x=(int)( i   *fmeasured_point.x/len_steps);
                    coord_free.y=(int)((i++)*fmeasured_point.y/len_steps);
                    char previous_pixel_type =
                        get_char(robot_coord_in_scanResult_str[0]-coord_free.y,
                                 robot_coord_in_scanResult_str[1]+coord_free.x,
                                 scanResult);
                    if(previous_pixel_type == WALL_PIXEL) continue; //if it's already a wall, ignore
                    if(last_coord_free.x == 0
                        || point_eq(last_coord_free, coord_free))
                    {
                        last_coord_free = coord_free;
                        set_char(robot_coord_in_scanResult_str[0]-coord_free.y,
                                 robot_coord_in_scanResult_str[1]+coord_free.x,
                                 FREE_PIXEL,
                                 scanResult);
                    }
                }
            } else
            {
                Point O;
                O.x = 0;
                O.y = 0;
                Point s;
                int x = min(min(measured_point.x, last_point.x),0) ;
                int y = min(min(measured_point.y, last_point.y),0) ;
                while (x< max(max(measured_point.x, last_point.x),0))
                while (x< max(max(measured_point.y, last_point.y),0))
                {
                    s.x = x;
                    s.y = y;
                    if(intpoint_in_trigon(s, O, measured_point, last_point))
                    {
                        set_char(robot_coord_in_scanResult_str[0]-s.y,
                             robot_coord_in_scanResult_str[1]+s.x,
                             FREE_PIXEL,
                             scanResult);
                    }
                }
            }
        }
    }
    free_isolated_cells(scanResult);
    stop_mov_motors();
    // free(buffer); //prevent memory leaks
    return scanResult;
}
