/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   08/01/2018
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   amrsoll
 * @Last modified time: 08/01/2018
 */



#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include "gsyst.h" /* Also contains the ev3 libraries*/
#include "classes.m"
#include "constants.m"

#include "map.h"

int set_char(int i, int j, int width, int height, char value, char* str) //Changes the char value str on line i and j
{
    if(i>=height){
        printf("index out of range : i = %d and height = %d\n", i, height);
        return -1;
    }
    if(j>=width){
        printf("index out of range : j = %d and width = %d\n", j, width);
        return -1;
    }
    int k;
    str[i*width+j] = value;
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

char* get_new_local_map(int width, int height)
//be sure to free the output when finished using #memory leak
{
    char* output = NULL;
    int k=0;
    output = malloc((width+1)*height*sizeof(char));
    if (output == NULL) exit(-1); // Error : failed to allocate memory
    int i=0;
    while(i<height)
    {
        int j=0;
        while(j<width) {
            output[k] = UNDEFINED_PIXEL;
            k++;
            j++;
        }
        i++;
        if(i<height) output[k] = '\n';
        k++;
    }
    return output;
}

void free_isolated_cells(char* map)
//if a pixel is directly surrounded by four pixels of a different type,
//then turn it into that type
{
    // TODO
    // be careful with borders
}

char* scan() //returns the string result of the scan
//remember to free the returned value
{
    float step_fill_free_space = 0.2;
    int mm_to_pixel_size = 10*PIXEL_SIZE;
    int width  = 2*SCANNING_MAX_DISTANCE/mm_to_pixel_size+2; //adding one in case of debordement
    int height = 2*SCANNING_MAX_DISTANCE/mm_to_pixel_size+2;
    char* scanResult = get_new_local_map(width, height);
    printf("map : \n%s\n", scanResult);
    int robot_coord_in_scanResult_str[2] = {width/2,height/2};
    float starting_scan_angle = angle; //angle is created in contants and constantly refreshed
    // float* buffer = NULL;
    // buffer = malloc(10*sizeof(int)/SCANNING_SPEED);
    // if (buffer == NULL) exit(-1); // Error : failed to allocate memory
    // unsigned short int buffer_index = 0;
    Point last_point; //so as to not constantly change the same pixel
    last_point.x = 0;
    Point measured_point; //coordinates of the tile/pixel the sonar hits into.
    start_turn(SCANNING_SPEED);
    float distance;
    printf("starting_scan_angle : %f\n", starting_scan_angle);
    printf("init_angle : %f\n", angle);
    while(abs(starting_scan_angle-angle)<360)
    {
        angle = get_angle() - init_angle;
        printf("angle : %f\n", angle);
        distance = min(SCANNING_MAX_DISTANCE, get_distance());
        fPoint fmeasured_point = {(distance*cos(angle/pi)/mm_to_pixel_size),
                                (distance*sin(angle/pi)/mm_to_pixel_size)};
        Point measured_point;
        measured_point.x =(int)fmeasured_point.x;
        measured_point.y =(int)fmeasured_point.y;
        printf("last_point.x : %d\n", last_point.x);
        if( last_point.x == 0
            || !point_eq(last_point, measured_point) )
        {
            last_point = measured_point;
            set_char(robot_coord_in_scanResult_str[0]-measured_point.y,
                     robot_coord_in_scanResult_str[1]+measured_point.x,
                     width,
                     height,
                     WALL_PIXEL,
                     scanResult);
            printf("I have set the wall pixel : %d : %d\n",
                measured_point.x ,
                measured_point.y);
            //fill the pixels between the wall and you with free space
            //if it's not already a wall or free space
            if( last_point.x==0 )
            {
                int len_steps = (int)distance/mm_to_pixel_size;
                Point coord_free;
                Point last_coord_free;
                last_coord_free.x = 0;
                int i=0;
                printf("len steps : \n", len_steps);
                while(i<len_steps)
                {
                    printf("step number : %d\n", i);
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
                printf("I have set the free pixels (first ones)");
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
                printf("I have set the free pixels (trigon)");
            }
        }
    }
    free_isolated_cells(scanResult);
    stop_mov_motors();
    // free(buffer); //prevent memory leaks
    return scanResult;
}
