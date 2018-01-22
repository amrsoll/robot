/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   08/01/2018
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   madafaka
 * @Last modified time: 19/01/2018
 */



#ifndef MAP_H
#define MAP_H
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gsyst.h" /* Also contains the ev3 libraries*/
#include "classes.h"
#include "spot.h"
#include "constants.m"

char* get_new_local_map(int, int);
void free_isolated_cells(tCoord, char*, int, int);
void free_pixels_between(fPoint, fPoint,int,int, char* );
void free_pixels_in_trigon(Point, Point, Point,int,int, char* );
char* scan(fPoint, tCoord, int, int, char*);
int moveTo(tCoord, tCoord, int, int, char*);
int mapComplete(int,int,char*);

/* add the signature of any new functions from the .c file here */

#endif
