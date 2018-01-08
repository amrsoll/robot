#ifndef MAP_H
#define MAP_H
/* do not include any other librairies in this file */

int map(int,int);

int countlines(FILE);

Node getNode(int);

int addNode(Node);
char* load_map(void);
int addVertice(Node,Node);

int eqNodes(Node,Node);

int makeClockwise(char*);

int clean(void);
/* add the signature of any new functions from the .c file here */

#endif
