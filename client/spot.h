/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   17/12/2017
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   amrsoll
 * @Last modified time: 14/01/2018
 */



#ifndef SPOT_H
#define SPOT_H
#include <string.h>
#include "constants.m"
#include "classes.h"

#define DIST_MIN_BETWEEN_SCAN_POS_cm = 40;
#define DIST_MIN_BETWEEN_SCAN_POS = DIST_MIN_BETWEEN_SCAN_POS_cm/PIXEL_SIZE; //multiple of pixels.
#define DIST_MIN_FROM_WALLS_cm = 15;
#define DIST_MIN_FROM_WALLS = DIST_MIN_FROM_WALLS_cm/PIXEL_SIZE ; //multiple of pixels.
#define CONNEX_MAX_BUFFER_SIZE = MAP_WIDTH*MAP_WIDTH;

int number_of_set_bits(int);
void get_neighbours_of_same_char(tCoord,char,tCoord*,int,int,char*);
bool tCoord_explored(tCoord,tCoord*,size_t);
void get_connex_tCoord_of_same_char(tCoord,char,int,int,char*,tCoord*,size_t*);
tCoord getNewSpot(void); //TODO

/* add the signature of any new functions from the .c file here */

#endif
