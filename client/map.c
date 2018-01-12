/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   08/01/2018
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   amrsoll
 * @Last modified time: 12/01/2018
 */



#include "map.h"


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
    Point O = Point_new(0,0);
    int mm_to_pixel_size = 10*PIXEL_SIZE;
    float distance_before_setting_a_new_wall = SCANNING_MAX_DISTANCE - .4;
    int width  = 2*SCANNING_MAX_DISTANCE/mm_to_pixel_size+2; //adding one in case of debordement
    int height = 2*SCANNING_MAX_DISTANCE/mm_to_pixel_size+2;
    char* scanResult = get_new_local_map(width, height);
    width++; //add the \n for the set/get char functions
    tCoord robot_coord_in_scanResult_str = tCoord_new(width/2,height/2);

    // float* buffer = NULL;
    // buffer = malloc(10*sizeof(int)/SCANNING_SPEED);stdbool.h
    // if (buffer == NULL) exit(-1); // Error : failed to allocate memory
    // unsigned short int buffer_index = 0;
    Point measured_point; //coordinates of the tile/pixel the sonar hits into.
    // last_point will take the value of previously measured point.
    Point last_point =  Point_new(0,0); //initialised like 0 so as to know if nothing has been measured before

    start_turn(SCANNING_SPEED);
    refresh_angle();
    float starting_scan_angle = angle; //angle is created in contants and constantly refreshed
    printf("starting_scan_angle : %f\n", starting_scan_angle);
    tCoord coord;
    while(abs(starting_scan_angle-angle)<FULL_TURN_ANGLE)
    {
        // TODO : manual or clock interpolation to counter the wall curvatures.
        refresh_angle();
        refresh_distance();
        distance = min(SCANNING_MAX_DISTANCE, distance);
        // TODO : adapt speed to the measured distance (precision)
        fPoint fmeasured_point =
            fPoint_new(distance*cos(2*pi*angle/FULL_TURN_ANGLE)/mm_to_pixel_size,
                       distance*sin(2*pi*angle/FULL_TURN_ANGLE)/mm_to_pixel_size);
        measured_point = fPoint_to_Point(fmeasured_point);
        coord = Point_to_tCoord(measured_point, robot_coord_in_scanResult_str);
        if( Point_eq(last_point,O) || !Point_eq(last_point, measured_point) )
        //so as to not constantly change the same pixels (optimisation)
        {
            //shuffle between the boxes contained in the smallest rectangle \
            consisting of O (robot position), measured_point and last_point
            Point s = Point_new(min(min(measured_point.x, last_point.x),0),
                                min(min(measured_point.y, last_point.y),0));
            int sy_init=s.y;
            if( Point_eq(last_point,O) )
            {
                while (s.x< max(max(measured_point.x, last_point.x),0))
                {
                    while (s.y< max(max(measured_point.y, last_point.y),0))
                    {
                        //TODO : set a maximum limit to where the scan can overwrite\
                         pixels that were already known (precision)
                        tCoord ts = Point_to_tCoord(s,robot_coord_in_scanResult_str);
                        //if it is the first measure, last_point == O
                        //fill the pixels between the wall and you with free space
                        if(intsquare_fray_intersect(s,Point_to_fPoint(O),fmeasured_point))
                            set_char(ts,width,height,FREE_PIXEL,scanResult);
                    }
                    s.y++;
                }
                s.y = sy_init;
                s.x++;
            } else
            {
                while (s.x< max(max(measured_point.x, last_point.x),0))
                {
                    while (s.y< max(max(measured_point.y, last_point.y),0))
                    {
                        //free all the pixels contained in the triangle defined by
                        //the two last measured points and the position of the robot
                        if(intpoint_in_trigon(s, O, measured_point, last_point))
                            set_char(ts,width,height,FREE_PIXEL,scanResult);
                    }
                    s.y++;
                }
                s.y = sy_init;
                s.x++;
            }
            // And set the furthest pixel to become a wall if appropriate.
            if(distance < distance_before_setting_a_new_wall) //-.4 because of rounding errors
            {
                set_char(coord,width,height,WALL_PIXEL,scanResult);
                printf("wall pixel set\n" );
            }
            last_point = measured_point;
        }
    }
    // smooth the map out / remove scattered pixels
    free_isolated_cells(scanResult);
    // mark the position of the robot during the scan
    set_char(robot_coord_in_scanResult_str,width,height,'a',scanResult);
    stop_mov_motors();
    // free(buffer); //prevent memory leaks
    return scanResult;
}
