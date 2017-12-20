#include "servercom.h"

int main(int argc, char **argv) {
    int s; /* socket */
    uint16_t msgId = 0; /* msg seq num */

    //printf("hello\n");

    int16_t posX, posY;

    /* SET UP BT CONNECTION TO SERVER */
    struct sockaddr_rc addr = { 0 };
    int status;

    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba (SERV_ADDR, &addr.rc_bdaddr);

    /* connect */
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    printf("status: %d\n", status);
    /* if connected */
    if(status==0) {
        char string[58];

        /* wait for START message */
        read_from_server(s, string,9);
        printf("string: %s\n", string);
        if(string[4] == MSG_START) {
            printf("Received start message!\n");
        }

        int i;
        posX=0x00;
        posY=0x00;

        for (i=0;i<30;i++) {
            send_POSITION(posX, posY);
            posX++;
            posY++;
            Sleep(2000);
        }

        while(1) {
            read_from_server(s, string, 58);
            if(string[4] == MSG_STOP) {
                printf("STOP received!");
                break;
            }
            else if(string[4] == MSG_KICK && string[5] == TEAM_ID) {
                printf("We got kicked! :(");
                break;
            }
        }
    } else {
        fprintf(stderr, "Failed to connect to server...\n");
        sleep(2);
        exit(EXIT_FAILURE);
    }

    close(s);
    return 0;
}
