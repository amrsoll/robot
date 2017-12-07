#include "gsyst.h"
#include "map.h"
#include "path.h"
#include "spot.h"
#include "servercom.h"


int clean() {
    /*Cleans the memory for a new round of mapping*/
    remove("map/nodes");
    remove("map/vertices");

}

int main(int argc, char **argv) {

}
