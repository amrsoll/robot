/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   08/01/2018
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   madafaka
 * @Last modified time: 15/01/2018
 */



#ifndef CLASSES
#define CLASSES
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct Point Point;
struct Point {
    int x;
    int y;
};

//named for coordibates in a table
typedef struct tCoord tCoord;
struct tCoord {
    int i;
    int j;
};

typedef struct fPoint fPoint;
struct fPoint {
    float x;
    float y;
};


typedef struct Pixel Pixel;
struct Pixel {
    int x;
    int y;
    char type; //undefined : 0, clear : 1 or wall : 2
};

Point Point_new(int,int);
tCoord tCoord_new(int,int);
fPoint fPoint_new(float,float);
Pixel Pixel_new(int,int,char);
void fprintf_tCoord_to_str(FILE*,tCoord);
tCoord tCoord_init_str(char*,size_t);
Point fPoint_to_Point(fPoint);
fPoint Point_to_fPoint(Point);
tCoord Point_to_tCoord(Point, tCoord);
Point tCoord_to_Point(tCoord, tCoord);
//converts the cordinates of a point from a cartesian plot
//to table coordinates, where origin is the position of the
//cartesian plot's origin point inside the table.
bool intsquare_fray_intersect(Point,fPoint,fPoint);
bool pixel_eq(Pixel, Pixel);
bool point_eq(Point, Point);
float intnorm(Point);
float fnorm(fPoint);
float tcnorm(tCoord);
//
// fPoint fproject_onto(fPoint, fPoint);
float sign (fPoint,fPoint,fPoint);
bool fPoint_in_trigon (fPoint,fPoint,fPoint,fPoint);
bool intpoint_in_trigon (Point,Point,Point,Point);
int set_char(tCoord,int,int,char,char*);
char get_char(tCoord,int,int,char*);

#endif
