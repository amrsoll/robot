/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   08/01/2018
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   amrsoll
 * @Last modified time: 09/01/2018
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

int set_char(tCoord,int,int,char,char*);
char get_char(tCoord,int,int,char*);
char* get_new_local_map(int, int);
void free_isolated_cells(char*);
char* scan();
/* add the signature of any new functions from the .c file here */

#endif
