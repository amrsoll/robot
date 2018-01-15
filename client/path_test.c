/**
 * @Author: Axel_Soll <madafaka>
 * @Date:   15/01/2018
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   madafaka
 * @Last modified time: 15/01/2018
 */

#include "path.h"

#define TEST

int main(int argc, char **argv) {
    char* map =
    "#    ###      #\n#    ###      #\n#    ###      #\n#    ###      #\n#    ###      #\n#    ###      #\n#             #\n#    ###      #\n#    ###      #\n";
    /*
    "#    ###      #\n
    #    ###      #\n
    #    ###      #\n
    #    ###      #\n
    #    ###      #\n
    #    ###      #\n
    #             #\n
    #    ###      #\n
    #    ###      #\n"
    */

    int width = 15;
    int height = 9;
    tCoord start_coord = tCoord_new(2,2);
    tCoord goal_coord = tCoord_new(6,12);
    getPathTo(start_coord, goal_coord, width, height, map);


}
