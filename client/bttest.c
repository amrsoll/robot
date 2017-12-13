#include "servercom.h"


int main() {
    int status;
    int msg;

    int16_t posX, posY;

    status = connect_to_server();
    msg = parse_message();
    
    sleep(5);

    return 0;


}
