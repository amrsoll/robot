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
    startBt();
    getStartSignal(conn); /*blocking function. Does not continue without getting signal*/
    int x = 0;
    int y = 0;
    /*fork a process that will ping the server every 2 sec with the position of the robot*/
    while(!mapComplete()) /*counterintuitive : in C, the while loop continues as long as it is given an int !=0*/
    {
        sendPosition(x, y);
        if(map()){
            printf("failed to map the surroundings");
            exit(EXIT_FAILURE);}
        int* spot = getNewSpot();
        if(getPathTo(*spot[0],*spot[1])) {
            printf("failed to find the path to the new coordinates");
            exit(EXIT_FAILURE);}
        FILE *path = open("~/path");
        int pathLen = countlines(path);
        for(int i=0; i<pathLen; i++)
        {
            int k = n-i;
            nextNode = node_init_str(readline(*path, k));
            if(!(x,y = moveTo(x, y, nextNode.x, nextNode.y)){ /*error prone*/
                printf("failed to move to the next coordinates");
                exit(EXIT_FAILURE);}
            if(k>0)
                sendPosition(x, y);
        }
    }
    sendMap();
}
