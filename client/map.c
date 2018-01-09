/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   08/01/2018
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   amrsoll
 * @Last modified time: 09/01/2018
 */



#include "map.h"

int set_char(tCoord coord, int width, int height, char value, char* str) //Changes the char value str on line i and j
{
    if(coord.i>=height){
        printf("set_char : index out of range : i = %d and height = %d\n", coord.i, height);
        return -1;
    }
    if(coord.j>=width){
        printf("set_char : index out of range : j = %d and width = %d\n", coord.j, width);
        return -1;
    }
    str[coord.i*width+coord.j] = value;
    return 0;
}

char get_char(tCoord coord, int width, int height, char* str)
//i0 and j0 is the position of the origin of the x and y axis
{
    if(coord.i>=height){
        printf("get_char : index out of range : i = %d and height = %d\n", coord.i, height);
        return -1;
    }
    if(coord.j>=width){
        printf("get_char : index out of range : j = %d and width = %d\n", coord.j, width);
        return -1;
    }
    return str[coord.i*width+coord.j];
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
    Point O = Point_new(0,0);
    float step_fill_free_space = 0.2;
    int mm_to_pixel_size = 10*PIXEL_SIZE;
    int width  = 2*SCANNING_MAX_DISTANCE/mm_to_pixel_size+2; //adding one in case of debordement
    int height = 2*SCANNING_MAX_DISTANCE/mm_to_pixel_size+2;
    char* scanResult = get_new_local_map(width, height);
    width++; //add the \n for the set/get char functions
    tCoord robot_coord_in_scanResult_str = tCoord_new(width/2,height/2);

    // float* buffer = NULL;
    // buffer = malloc(10*sizeof(int)/SCANNING_SPEED);stdbool.h
    // if (buffer == NULL) exit(-1); // Error : failed to allocate memory
    // unsigned short int buffer_index = 0;
    Point last_point = Point_new(0,0); //so as to not constantly change the same pixel
    Point measured_point; //coordinates of the tile/pixel the sonar hits into.
    start_turn(SCANNING_SPEED);
    refresh_angle();
    float starting_scan_angle = angle; //angle is created in contants and constantly refreshed
    printf("starting_scan_angle : %f\n", starting_scan_angle);
    tCoord coord;
    while(abs(starting_scan_angle-angle)<FULL_TURN_ANGLE)
    {
        refresh_angle();
        refresh_distance();
        printf("angle : %f \t\t Sonar : %f\n", angle, distance);
        distance = min(SCANNING_MAX_DISTANCE, distance);
        fPoint fmeasured_point =
            fPoint_new(distance*cos(2*pi*angle/FULL_TURN_ANGLE)/mm_to_pixel_size,
                       distance*sin(2*pi*angle/FULL_TURN_ANGLE)/mm_to_pixel_size);
        measured_point = fPoint_to_Point(fmeasured_point);
        coord = Point_to_tCoord(measured_point, robot_coord_in_scanResult_str);
        if( Point_eq(last_point,O) || !Point_eq(last_point, measured_point) )
        {

            //printf("I have set the wall pixel (%d : %d)\n",
                // measured_point.x ,
                // measured_point.y);
            //printf("last_point.x : %d\n", last_point.x);
            //fill the pixels between the wall and you with free space
            //if it's not already a wall or free space
            //optimisation : s is the same wether last point is equal to O or not
            Point s = Point_new(min(min(measured_point.x, last_point.x),0),
                                min(min(measured_point.y, last_point.y),0));
            int sy=s.y;
            //printf("breack1\n");
            while (s.x< max(max(measured_point.x, last_point.x),0))
            {
                //printf("   breack2\n");
                while (s.y< max(max(measured_point.y, last_point.y),0))
                {
                    //printf("        breack3\n");
                    tCoord ts = Point_to_tCoord(s,robot_coord_in_scanResult_str);
                    // if(get_char(ts,width,height,scanResult)==WALL_PIXEL)
                    //     continue;
                    if( !Point_eq(last_point,O) ) {
                        if(intpoint_in_trigon(s, O, measured_point, last_point))
                            set_char(ts,width,height,FREE_PIXEL,scanResult);
                    }
                    else{
                        if(intsquare_fray_intersect(s,Point_to_fPoint(O),fmeasured_point))
                            set_char(ts,width,height,FREE_PIXEL,scanResult);
                    }

                    s.y++;
                }
                s.y = sy;
                s.x++;
            }
            last_point = measured_point;
            if(distance > SCANNING_MAX_DISTANCE - 1.0)
                set_char(coord,width,height,FREE_PIXEL,scanResult);
            else
            {
                set_char(coord,width,height,WALL_PIXEL,scanResult);
                printf("wall pixel set\n" );
            }
        }
    }
    set_char(robot_coord_in_scanResult_str,width,height,'a',scanResult);
    free_isolated_cells(scanResult);
    stop_mov_motors();
    // free(buffer); //prevent memory leaks
    return scanResult;
}
