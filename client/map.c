/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   08/01/2018
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   madafaka
 * @Last modified time: 19/01/2018
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


void free_isolated_cells(tCoord cell, char* map, int width, int height)
//if a pixel is directly surrounded by four pixels of a different type,
//then turn it into that type
{
    int last_row = height*width;
    tCoord output[4];
    char c;
    if (cell.i!=0 && cell.j!=width && cell.i!=last_row && cell.j!=0) {
        get_neighbours_of_same_char(cell, c, output, width, height, map);
        if (get_char(output[0], width, height, map) == get_char(output[1], width, height, map)
        == get_char(output[2], width, height, map) == get_char(output[3], width, height, map)
        != get_char(cell, width, height, map)) {
            char value = get_char(output[0], width, height, map);
            set_char(cell, width, height, value, map);
        }
    }
}

void free_pixels_between(fPoint p1, fPoint p2, int width, int height, char* map)
{
    Point p = Point_new(min((int)p1.x, (int)p2.x),
                        min((int)p1.y, (int)p2.y));
    int py_init=p.y;
    while (p.x< max((int)p1.x, (int)p2.x))
    {
        while (p.y< max((int)p1.x, (int)p2.x))
        {
            //TODO : set a maximum limit to where the scan can overwrite\
             pixels that were already known (precision)
            //if it is the first measure, p2 == O
            //fill the pixels between the wall and you with free space
            if(intsquare_fray_intersect(p,p1,p2))
                set_char(Point_to_tCoord(p,start_position)
                        ,width,height
                        ,FREE_PIXEL
                        ,map);
            p.y++;
        }
        p.y = py_init;
        p.x++;
    }
}

void free_pixels_in_trigon(Point p1, Point p2, Point p3, int width, int height, char* map)
{
    Point p = Point_new(min(min(p3.x, p2.x),p1.x),
                        min(min(p3.y, p2.y),p1.y));
    int py_init=p.y;
    while (p.x< max(max(p3.x, p2.x),p1.x))
    {
        while (p.y< max(max(p3.y, p2.y),p1.y))
        {
            //free all the pixels contained in the triangle defined by
            //the two last measured points and the position of the robot
            if(intpoint_in_trigon(p, p1, p2, p3))
                set_char(Point_to_tCoord(p,start_position)
                        ,width
                        ,height
                        ,FREE_PIXEL
                        ,map);
            p.y++;
        }
        p.y = py_init;
        p.x++;
    }
}

char* scan(fPoint robotPosition, tCoord start_position, int width, int height, char* map) //returns the string result of the scan
//remember to free the returned value
{
    Point O = fPoint_to_Point(robotPosition);
    int mm_to_pixel_size = 10*PIXEL_SIZE;
    float distance_before_setting_a_new_wall = SCANNING_MAX_DISTANCE - .4;

    // float* buffer = NULL;
    // buffer = malloc(10*sizeof(int)/SCANNING_SPEED);
    // if (buffer == NULL) exit(-1); // Error : failed to allocate memory
    // unsigned short int buffer_index = 0;
    Point measured_point; //coordinates of the tile/pixel the sonar hits into.
    // last_point will take the value of previously measured point.
    Point last_point =  fPoint_to_Point(robotPosition); //initialised like 0 so as to know if nothing has been measured before

    start_turn(SCANNING_SPEED);
    refresh_angle();
    float starting_scan_angle = angle; //angle is created in contants and constantly refreshed
    printf("starting_scan_angle : %f\n", starting_scan_angle);
    tCoord coord;
    while(abs(starting_scan_angle-angle)<FULL_TURN_ANGLE)
    {
        // TODO : manual or clock interpolation to counter the wall curvatures.
        get_sensor_value0(sn_gyr,&angle);
        angle = angle - init_angle;
        printf("breack 1 : angle : %f\n", angle);
        // buffering the angle creates curvature lines in the scan
        refresh_distance();
        distance = min(SCANNING_MAX_DISTANCE, distance);
        // TODO : adapt speed to the measured distance (precision)
        fPoint fmeasured_point =
            fPoint_new(distance*MM_TO_PIX_SIZE*cos(2*pi*angle/FULL_TURN_ANGLE),
                       distance*MM_TO_PIX_SIZE*sin(2*pi*angle/FULL_TURN_ANGLE));
        measured_point = fPoint_to_Point(fmeasured_point);
        coord = Point_to_tCoord(measured_point, start_position);
        if( Point_eq(last_point,O) || !Point_eq(last_point, measured_point) )
        //so as to not constantly change the same pixels (optimisation)
        {
            //shuffle between the boxes contained in the smallest rectangle \
            consisting of O (robot position), measured_point and last_point

            if( Point_eq(last_point,O) )
            {
                free_pixels_between(Point_to_fPoint(O), fmeasured_point, width, height, map);
            } else
            {
                free_pixels_in_trigon(O,last_point, measured_point, width, height, map);
            }
            // And set the furthest pixel to become a wall if appropriate.
            if(distance < distance_before_setting_a_new_wall) //-.4 because of rounding errors
            {
                set_char(coord,width,height,WALL_PIXEL,map);
                printf("wall pixel set\n" );
            }
            last_point = measured_point;
        }
    }
    // smooth the map out / remove scattered pixels
    //free_isolated_cells(map);
    // mark the position of the robot during the scan
    // set_char(start_position,width,height,'a',map);
    stop_mov_motors();
    // free(buffer); //prevent memory leaks
    return map;
}

int moveTo(tCoord tc, tCoord start_position, int width, int height, char* map)
{
    Point goal =  tCoord_to_Point(tc, start_position);
    fPoint fgoal =  fPoint_new(goal.x/MM_TO_PIX_SIZE,
                               goal.y/MM_TO_PIX_SIZE);
    fPoint vector = fsub(fgoal, robotPosition);
    float vector_angle = (float)atan2(vector.y,vector.x);
    turn_to_angle(vector_angle);
    int collision = moveThisDistance(fnorm(vector));
    while(collision)
    {
        if(collision==-1)
        {
            sleep(5000);
            collision = moveThisDistance(fnorm(vector));
        } else
        if(collision==-2)
        {
            //better to scan probs
            Point wall = fPoint_to_Point(fPoint_new(distance*MM_TO_PIX_SIZE*cos(2*pi*angle/FULL_TURN_ANGLE),
                                                    distance*MM_TO_PIX_SIZE*sin(2*pi*angle/FULL_TURN_ANGLE)));
            set_char(Point_to_tCoord(wall,start_position),width,height,FREE_PIXEL,map);
            collision = moveThisDistance(fnorm(vector));
        }
    }
}

int mapComplete(int width, int height, char* map)
{
    //TODO : if all of the free cells do not have an unknown
    //cell as a neighbour, then the map is complete
    int i;
    int last_cell = height*width;
    tCoord coord;
    // for (i=0, i<last_cell, i++) {
    // if (get_char(map[i], width, height, map) == FREE_PIXEL) {
    //     char c;
    //     tCoord output[4];
    //     get_neighbours_of_same_char(map[i], c, output, width, height, map);
    //     if (   get_char(output[0], width, height, map) != UNDEFINED_PIXEL
    //         || get_char(output[1], width, height, map) != UNDEFINED_PIXEL
    //         || get_char(output[2], width, height, map) != UNDEFINED_PIXEL
    //         || get_char(output[3], width, height, map) != UNDEFINED_PIXEL) {
    //     return false; }//map is not
    //   }
    // }
    return true;
}
