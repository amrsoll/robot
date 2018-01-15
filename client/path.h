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
#include "classes.h"
/* do not include any other librairies in this file */
void append(char*, char);
int countlines(FILE*);
char* getLine(FILE*, int);
int getPathTo(tCoord);
tCoord getCheckpoint(int,int);

// Node of the adjacency list
struct node {
    int vertex, weight;
    struct node * next;
};

struct node * addEdge(struct node * head, int vertex, int weight)
int getMinVertex(int distances[], int visited[], int vertices)
void dijkstra(struct node * adjacencyList[], int vertices, int startVertex, int distances[], int parent[])
void printPath(int parent[], int vertex, int startVertex)
int get_pos_in_list(tCoord* coord_correspond, tCoord coord)


/* add the signature of any new functions from the .c file here */

#endif
