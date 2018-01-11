/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   08/01/2018
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   amrsoll
 * @Last modified time: 11/01/2018
 */



#ifndef CLASSES
#define CLASSES
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "constants.m"
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
Point fPoint_to_Point(fPoint);
fPoint Point_to_fPoint(Point);
tCoord Point_to_tCoord(Point p, tCoord origin);
//converts the cordinates of a point from a cartesian plot
//to table coordinates, where origin is the position of the
//cartesian plot's origin point inside the table.
bool intsquare_fray_intersect(Point,fPoint,fPoint);
bool pixel_eq(Pixel p1, Pixel p2);
bool point_eq(Point p1, Point p2);
// float norm(fPoint);
//
// fPoint project_onto(fPoint, fPoint);
float sign (fPoint,fPoint,fPoint);
bool fPoint_in_trigon (fPoint,fPoint,fPoint,fPoint);
bool intpoint_in_trigon (Point,Point,Point,Point);
int set_char(tCoord,int,int,char,char*);
char get_char(tCoord,int,int,char*);

#endif
