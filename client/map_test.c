#include <stdio.h>
#include <stdlib.h>
#include "gsyst.h" /* Also contains the ev3 libraries*/
#include "map.h"


#define SCAN_TEST


int main(int argc, char **argv) {
    float init_angle = get_angle();
    float angle = init_angle;
    float x = 0.0;
    float y = 0.0;

#ifdef SCAN_TEST
    init_mov_motors();
    char* scanResult = scan();
    printf("%s\n",scanResult );
    free(scanResult); //prevent memory leaks
#endif

}
