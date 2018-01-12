/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   19/12/2017
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   amrsoll
 * @Last modified time: 12/01/2018
 */

#include "spot.h"

int number_of_set_bits(int i)
//found on stackoverflow
{
     i = i - ((i >> 1) & 0x55555555);
     i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
     return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

tCoord* get_neighbours_of_same_char(tCoord tc, char c, int width, int height, char* map)
{
    int* offset[3] = {-1,0,1};
    int i=0;
    tCoord neighbour;
    int j=0;
    while(i<4)
    {
        neighbour = tCoord_new(tc.i+offset[i%3], tc.j+offset[(i+1)%3]);
        if(c==get_char(neighbour,width,height,map)
            &&neighbour.i>-1
            &&neighbour.i<height
            &&neighbour.j>-1) // no &&neighbour.j<width because of \n
            j|=1;
        j<<=1;
        i++;
    }
    int k = numberOfSetBits(j)+1
    tCoord* neighbours[k];
    k--;
    neighbours[k] = tc;
    while(i>0){
        if(j&1){
            k--;
            neighbours[k]=tCoord_new(tc.i+offset[i%3], tc.j+offset[(i--+1)%3]);
        }
        j>>=1;
    }
    return neighbours;
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
    if(!tCoord_explored(tc, connextCoords, &sizeof_connextCoords))
    {
        connextCoords[sizeof_connextCoords] = tc;
        sizeof_connextCoords++;
        tCoord* neighbours = get_neighbours_of_same_char(tc,c, width, height, map);
        int i=0;
        while(!tCoord_eq(neighbours[i],tc))
        {
            get_connex_tCoord_of_same_char(tc[i], c, width, height, map, connextCoords, sizeof_connextCoords);
            i++;
        }
    }
}

int* getNewSpot()
{
    /*TODO*/
}
