/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   19/12/2017
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   amrsoll
 * @Last modified time: 13/01/2018
 */

#include "spot.h"


int number_of_set_bits(int i)
//found on stackoverflow
{
     i = i - ((i >> 1) & 0x55555555);
     i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
     return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

void get_neighbours_of_same_char(tCoord tc, char c,tCoord* output, int width, int height, char* map)
{
    int offset[4] = {-1,0,1,0};
    int i=0;
    tCoord neighbour;
    int j=0;
    printf("                  break 4\n");
    while(i<4)
    {
        neighbour = tCoord_new(tc.i+offset[i], tc.j+offset[(i+1)%4]);
        printf("####    break 5\n");
        if(neighbour.i>-1
           &&neighbour.i<height
           &&neighbour.j>-1
           &&c==get_char(neighbour,width,height,map)
        ) // no &&neighbour.j<width because of \n
            j|=1;
        printf("####    break 6\n");
        j<<=1;
        i++;
    }
    int k = number_of_set_bits(j)+1;
    //tCoord output[k];
    k--;
    output[k] = tc;
    printf("                 break 7\n");
    while(i>0){
        if(j&1){
            k--;
            output[k]=tCoord_new(tc.i+offset[i], tc.j+offset[(i+1)%4]);
            printf("####    break 8\n");
        }
        i--;
        j>>=1;
    }
}

bool tCoord_explored(tCoord tc, tCoord* connextCoords, size_t sizeof_connextCoords)
{
    int i=0;
    while(i<sizeof_connextCoords)
    {
        if(tCoord_eq(tc, connextCoords[i]))
            return true;
    }
    return false;
}

void get_connex_tCoord_of_same_char(tCoord tc,
                                   char c,
                                   int width,
                                   int height,
                                   char* map,
                                   tCoord* connextCoords,
                                   size_t* sizeof_connextCoords)
// tCoord* connextCoords[strlen(map)-height+1]; do not count the \n in the map
// size_t sizeof_connextCoords = 0;
{
    printf("break 1\n");
    if(!tCoord_explored(tc, connextCoords, *sizeof_connextCoords))
    {
        printf("      break 2\n");
        printf("      size_t sizeof_connextCoords : %d\n", (int)*sizeof_connextCoords );
        tCoord neighbourBuffer[4];
        connextCoords[(int)*sizeof_connextCoords] = tc;
        sizeof_connextCoords++;
        printf("              break 3\n");
        get_neighbours_of_same_char(tc,c, neighbourBuffer, width, height, map);
        int i=0;
            printf("                      break 9\n");
        while(!tCoord_eq(neighbourBuffer[i],tc))
        {
            get_connex_tCoord_of_same_char(neighbourBuffer[i], c, width, height, map, connextCoords, sizeof_connextCoords);
            i++;
        }
    }
}

int* getNewSpot()
{
    /*TODO*/
}
