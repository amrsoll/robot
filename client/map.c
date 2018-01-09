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
    float step_fill_free_space = 0.2;
    int mm_to_pixel_size = 10*PIXEL_SIZE;
    int width  = 2*SCANNING_MAX_DISTANCE/mm_to_pixel_size+2; //adding one in case of debordement
    int height = 2*SCANNING_MAX_DISTANCE/mm_to_pixel_size+2;
    char* scanResult = get_new_local_map(width, height);
    tCoord robot_coord_in_scanResult_str = tCoord_new(width/2,height/2);
    set_char(robot_coord_in_scanResult_str,width+1,height,'a',scanResult);

    // float* buffer = NULL;
    // buffer = malloc(10*sizeof(int)/SCANNING_SPEED);
    // if (buffer == NULL) exit(-1); // Error : failed to allocate memory
    // unsigned short int buffer_index = 0;
    Point last_point = Point_new(0,0); //so as to not constantly change the same pixel
    Point measured_point; //coordinates of the tile/pixel the sonar hits into.
    start_turn(SCANNING_SPEED);
    float distance;
    float starting_scan_angle = angle; //angle is created in contants and constantly refreshed
    printf("starting_scan_angle : %f\n", starting_scan_angle);
    tCoord coord;
    while(abs(starting_scan_angle-angle)<360)
    {
        angle = get_angle() - init_angle;
        printf("angle : %f\n", angle);
        distance = min(SCANNING_MAX_DISTANCE, get_distance());
        fPoint fmeasured_point =
                        fPoint_new(distance*cos(angle/pi)/mm_to_pixel_size,
                                   distance*sin(angle/pi)/mm_to_pixel_size);
        measured_point = fPoint_to_Point(fmeasured_point);
        printf("last_point.x : %d\n", last_point.x);
        coord = Point_to_tCoord(measured_point, robot_coord_in_scanResult_str);
        if( last_point.x == 0
            || !point_eq(last_point, measured_point) )
        {
            if(distance > SCANNING_MAX_DISTANCE - 1.0)
                set_char(coord,width+1,height,FREE_PIXEL,scanResult);
            else
                set_char(coord,width+1,height,WALL_PIXEL,scanResult);
            printf("I have set the wall pixel (%d : %d)\n",
                measured_point.x ,
                measured_point.y);
            printf("last_point.x : %d\n", last_point.x);
            //fill the pixels between the wall and you with free space
            //if it's not already a wall or free space
            if( last_point.x==0 )
            {
                printf("if trigger");
                int len_steps = (int)distance/mm_to_pixel_size;
                Point coord_free;
                Point last_coord_free;
                last_coord_free.x = 0;
                int i=0;
                printf("len steps : %d\n", len_steps);
                while(i<len_steps)
                {
                    printf("step number : %d\n", i);
                    coord_free.x=(int)( i   *fmeasured_point.x/len_steps);
                    coord_free.y=(int)((i++)*fmeasured_point.y/len_steps);
                    coord = Point_to_tCoord(coord_free,robot_coord_in_scanResult_str);
                    char previous_pixel_type = get_char(coord,width+1,height,scanResult);
                    if(previous_pixel_type == WALL_PIXEL) continue; //if it's already a wall, ignore
                    if(last_coord_free.x == 0
                        || !point_eq(last_coord_free, coord_free))
                    {
                        last_coord_free = coord_free;
                        set_char(coord,width+1,height,FREE_PIXEL,scanResult);
                    }
                }
                last_point = measured_point;
                printf("I have set the free pixels (first ones)\n");
            } else
            {
                Point O = Point_new(0,0);
                Point s = Point_new(min(min(measured_point.x, last_point.x),0),
                                    min(min(measured_point.y, last_point.y),0));
                while (s.x< max(max(measured_point.x, last_point.x),0))
                {
                    while (s.y< max(max(measured_point.y, last_point.y),0))
                    {
                        if(intpoint_in_trigon(s, O, measured_point, last_point))
                            set_char(
                                Point_to_tCoord(s,robot_coord_in_scanResult_str),
                                width+1,
                                height,
                                FREE_PIXEL,
                                scanResult);
                        s.y++;
                    }
                    s.x++;
                }
                printf("I have set the free pixels (trigon)");
                last_point = measured_point;
            }
        }
    }
    free_isolated_cells(scanResult);
    stop_mov_motors();
    // free(buffer); //prevent memory leaks
    return scanResult;
}
