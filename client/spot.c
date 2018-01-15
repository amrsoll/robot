/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   19/12/2017
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   madafaka
 * @Last modified time: 15/01/2018
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
    printf("                  break 4  %d , %d\n",tc.i,tc.j);
    while(i<4)
    {
        j<<=1;
        neighbour = tCoord_new(tc.i+offset[i], tc.j+offset[(i+1)%4]);
        if(neighbour.i>-1
           &&neighbour.i<height
           &&neighbour.j>-1
           &&c==get_char(neighbour,width,height,map)
        ) // no &&neighbour.j<width because of \n
        {
            //printf("####    break 5        %d , %d\n",neighbour.i,neighbour.j);
            j|=1;
        }
        i++;
    }
    int k = number_of_set_bits(j)-1;
    if(k<3)
    {
      output[k+1] = tc;
    }
    //printf("                 break 7   k=%d  ,  j=%d\n",k,j);
    while(i>0){
        if(j&1){
            output[k]=tCoord_new(tc.i+offset[i-1], tc.j+offset[i%4]);
            //printf("####    break 8  indice =%d\n",3-k);
            k--;
        }
        i--;
        j>>=1;
    }
    printf("output get_neighbours_of_same_char :   ");
    for(i=0;i<4;i++)
      printf("(%d , %d)  ", output[i].i,output[i].j);
    printf("\n");
}

bool tCoord_explored(tCoord tc, tCoord* connextCoords, size_t sizeof_connextCoords)
{
    int i=0;
    //printf("sizeof_connextCoords : %d , i \n", sizeof_connextCoords);
    while(i<(int)sizeof_connextCoords)
    {
        if(tCoord_eq(tc, connextCoords[i]))
        {
            // printf(" tCoord_explored : tc                  i=%d  ,  j=%d\n",tc.i,tc.j);
            // printf(" tCoord_explored : connextCoords[i]    i=%d  ,  j=%d\n",tc.i,tc.j);
            return true;
        }
        i++;
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
//does not work because of some strange overflow between the variables.
{
    printf("break 1             tc.i=%d  ,  tc.j=%d\n",tc.i,tc.j);
    if(!tCoord_explored(tc, connextCoords, *sizeof_connextCoords))
    {
        connextCoords[(int)*sizeof_connextCoords] = tc;
        sizeof_connextCoords[0]++;
        tCoord neighbourBuffer[4];
        printf("break 2                                 size_t sizeof_connextCoords : %d\n", (int)*sizeof_connextCoords );
        printf("break 2                                 connextCoords : " );
        int k;
        for(k=0;k<sizeof_connextCoords[0];k++)
          if(connextCoords[k].i<0 || connextCoords[k].i>9 || connextCoords[k].j<0 || connextCoords[k].j>9 || k>sizeof_connextCoords[0]-3)
            printf("(%d , %d)  ", connextCoords[k].i,connextCoords[k].j);
        printf("\n");
        get_neighbours_of_same_char(tc,c, neighbourBuffer, width, height, map);
        int i=0;
        while(!tCoord_eq(neighbourBuffer[i],tc) || i<4)
        {
            get_connex_tCoord_of_same_char(neighbourBuffer[i], c, width, height, map, connextCoords, sizeof_connextCoords);
            i++;
        }
    }
}

bool noCharInCircle(tCoord tcenter, int radius, char type, int width, int height, char* map) //it's actually a square LOL #Need4speed
{
    tCoord tc_test = tCoord_new(tcenter.i - radius,
                                tcenter.j - radius);
    for( ; tc_test.i<tcenter.i + radius ; tc_test.i++)
    for( ; tc_test.j<tcenter.j + radius ; tc_test.j++)
        if(get_char(tc_test,width, height, map) ==type)
            return false;
    return true;
}

tCoord getClosestCellFromList(tCoord tc, tCoord* tcells, int tcells_size)
{
    float min_dist = -1.0;
    tCoord closest_cell;
    int i = 0;
    while(i<tcells_size)
    {
        if(tcnorm(tsub(tc,tcells[i])) < min_dist || min_dist<0)
        {
            min_dist = tcnorm(tsub(tc,tcells[i]));
            closest_cell = tcells[i];
        }
    }
    return closest_cell;
}

tCoord getNewSpot(tCoord* previousSpots, int width, int height,char* map)
{
    tCoord tc, newSpot;
    int index_valid_pix = 0;
    int index_valid_pix_planB = 0;
    tCoord valid_free_pixels[width*height];
    tCoord valid_free_pixels_planB[width*height];

    for(tc.i=0 ; tc.i<height  ; tc.i++)
    for(tc.j=0 ; tc.j<width ; tc.j++)
    {
        char c = get_char(tc,width,height,map);
        if(c == FREE_PIXEL && tc.i >DIST_MIN_FROM_WALLS && tc.j >DIST_MIN_FROM_WALLS/2)
            if(noCharInCircle(tc, DIST_MIN_FROM_WALLS , WALL_PIXEL, width, height, map))
            {
                valid_free_pixels[index_valid_pix++] = tc;
                valid_free_pixels_planB[index_valid_pix_planB++] = tc;
            } else
            if(noCharInCircle(tc, DIST_MIN_FROM_WALLS/2 , WALL_PIXEL, width, height, map)){
                valid_free_pixels_planB[index_valid_pix_planB++] = tc;
            }
    }
    if(index_valid_pix==0 && index_valid_pix_planB==0)
    {
        printf("no more intersting spots left to scan from\n");
        exit(EXIT_FAILURE);
    } else
    if(index_valid_pix>0)
    {
        newSpot = getClosestCellFromList(Point_to_tCoord(fPoint_to_Point(robotPosition), start_position)
                                         , valid_free_pixels
                                         , index_valid_pix);
    } else
    {
        newSpot = getClosestCellFromList(Point_to_tCoord(fPoint_to_Point(robotPosition), start_position)
                                         , valid_free_pixels_planB
                                         , index_valid_pix_planB);
    }
    return newSpot;
}
