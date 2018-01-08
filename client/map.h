#ifndef MAP_H
#define MAP_H

/* do not include any other librairies in this file */

int set_char(int,int,int,int,char,char*);
char get_char(int,int,char*);
void free_isolated_cells(char*);
char* scan();
/* add the signature of any new functions from the .c file here */

#endif
