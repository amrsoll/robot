/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   08/01/2018
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   amrsoll
 * @Last modified time: 14/01/2018
 */



#ifndef MAP_H
#define MAP_H
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include "gsyst.h" /* Also contains the ev3 libraries*/
#include "classes.h"
#include "constants.m"

char* get_new_local_map(int, int);
void free_isolated_cells(char*);
void free_pixels_between(Point, Point, char* );
void free_pixels_in_trigon(Point, Point, Point, char* );
char* scan();
int mapComplete(char*);
/* add the signature of any new functions from the .c file here */

#endif
