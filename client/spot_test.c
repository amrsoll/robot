/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   11/01/2018
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   amrsoll
 * @Last modified time: 12/01/2018
 */

#include "spot.h"

#define CONNEX

int main(int argc, char **argv) {

#ifdef CONNEX
    int width = 10;
    int height= 10;
    tCoord origin = tCoord_new(2,2);
    char* map = "++++++++++\n++++++++++\n++++++++++\n++++++++++\n++++++++++\n++++++++++\n++++++++++\n++++++++++\n++++++++++\n++++++++++\n";
    width++;
    tCoord connextCoords[strlen(map)-height+1]; // do not count the \n in the map
    size_t sizeof_connextCoords[1] = {0};
    printf("map freed of connex: \n%s\n", map);

    get_connex_tCoord_of_same_char(tCoord_new(5,5),
                                   '+',
                                   width,
                                   height,
                                   map,
                                   connextCoords,
                                   sizeof_connextCoords);
    int i;
    for(i=0; i<*sizeof_connextCoords; i++)
    {
        set_char(connextCoords[i], width+1, height, ' ', map);
    }
    set_char(origin, width+1, height, 'a', map);
    printf("map freed of connex: \n%s\n", map);


#endif


}
