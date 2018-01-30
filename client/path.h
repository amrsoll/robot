/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   17/12/2017
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   madafaka
 * @Last modified time: 15/01/2018
 */



#ifndef PATH_H
#define PATH_H

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "classes.h"
#include "constants.m"
#include "spot.h"

void append(char*, char);
int countlines( FILE*);
int getLine( FILE*, int, char*);
int getPathTo(tCoord,tCoord, int, int, char*);
tCoord getCheckpoint(int,int);

// Node of the adjacency list
struct node {
    int vertex, weight;
    struct node * next;
};

struct node * addEdge(struct node*, int, int);
int getMinVertex(int* , int*, int);
void dijkstra(struct node**, int, int, int*, int*);
void fprintPath( FILE*,int*, int, int,tCoord*);
int get_pos_in_list(tCoord*,size_t, tCoord);
tCoord getCheckpoint(int, int);


/* add the signature of any new functions from the .c file here */

#endif
