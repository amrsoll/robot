/**
* @Author: Axel_Soll <amrsoll>
* @Date:   13/01/2018
* @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   amrsoll
 * @Last modified time: 14/01/2018
*/


#include "path.h"

void append(char* s, char c)
{
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}

char* getLine(FILE* f, int i)
{
    rewind(f);
    char* line[256];
    char ch;
    while(!feof(f)&&i>1é)
    {
        ch = fgetc(f);
        if(ch == '\n')
            i--;
    }
    if(feof(f))
    {
        printf("error : the file does not have that many lines\n");
        exit(EXIT_FAILURE);
    } else
    {
        while(!feof(f)&&ch!='\n')
        {
            ch = fgetc(f);
            append(line, ch);
        }
        return line;
    }
}


int getPathTo(tCoord)
//clears previous path file and creates a new one.
{
    //The file onto which we will write the nodes for when the robot moves around
    const FILE *path = open("~/path", O_RDWR ,0666);
    close(path);
    return 0;
}

tCoord getCheckpoint(int i, int pathLen)
{
    const FILE *path = open("~/path", O_RDWR ,0666);

    tCoord tc = tCoord_init_str(getLine(path, pathLen-i));
    close(path);
    return tc;
}
