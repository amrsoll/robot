/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   11/01/2018
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   amrsoll
 * @Last modified time: 11/01/2018
 */

#include "map.h"
#include "spot.h"

#define CONNEX

int main(int argc, char **argv) {

#ifdef CONNEX
    int width = 10;
    int height= 10;
    char* map = get_new_local_map(width, height);
    width++;
    tCoord* connextCoords[strlen(map)-height+1]; do not count the \n in the map
    size_t sizeof_connextCoords = 0;
    get_connex_tCoord_of_same_char(tCoord_new(5,5),
                                       char '+',
                                       int width,
                                       int height,
                                       char* map,
                                       tCoord origin,
                                       tCoord* connextCoords,
                                       size_t* sizeof_connextCoords)


#endif


}
