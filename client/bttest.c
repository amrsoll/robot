#include "servercom.h"


int main() {
    int status;
    int msg;

    int16_t posX, posY;

    status = connect_to_server();
    msg = parse_message();
    
    sleep(5);

    /*
    int i;
    posX=100;
    posY=100;
    for (i=0;i<30;i++) {
        
        send_POSITION(posX,posY);
        posX +=1;
        posY -=1;
    }
    */

    return 0;


}
