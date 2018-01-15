/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   17/12/2017
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   amrsoll
 * @Last modified time: 15/01/2018
 */



#ifndef PATH_H
#define PATH_H

#include <stdio.h>
#include <string.h>
#include "classes.h"
/* do not include any other librairies in this file */
void append(char*, char);
int countlines(FILE*);
char* getLine(FILE*, int);
int getPathTo(tCoord);
tCoord getCheckpoint(int,int);

/* add the signature of any new functions from the .c file here */

#endif
