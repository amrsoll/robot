#include "servercom.h"


volatile int DONE_EXPLORING = 0;
int16_t posX, posY;

/* dummy */
int setPosition() {
    posX += 1;
    posY += 2;
}

void *thSendPosition() {
    while(!DONE_EXPLORING) {
        Sleep(2000);
        setPosition();
        send_POSITION(posX, posY);
    }
    pthread_exit(NULL);
}

void *thReceiveFromServer() {
    while(!DONE_EXPLORING) {
        parse_message();
    }
    pthread_exit(NULL);
}

int main(int argc, char **argv) {

    /* start position */
    posX = 0;
    posY = 0;
    
    /* threads */
    pthread_t positioning;
    pthread_t receiving;

    /* SET UP BT CONNECTION TO SERVER */
    struct sockaddr_rc addr = { 0 };
    int status;

    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba(SERV_ADDR, &addr.rc_bdaddr);

    /* connect */
    status = connect(SOCKET, (struct sockaddr *)&addr, sizeof(addr));
    printf("status: %d\n", status);
    /* if connected */
    if(status==0) {
        char string[58];

        /* wait for START message */
        read_from_server(string,9);
        printf("string: %s\n", string);
        if(string[4] == MSG_START) {
            printf("Received start message!\n");
        }

        if(pthread_create(&positioning, NULL, thSendPosition, NULL)) {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }
        if(pthread_create(&receiving, NULL, thReceiveFromServer, NULL)) {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }
        printf("Threads created successfully\n");

        Sleep(20000);

        printf("Done exploring\n");
        DONE_EXPLORING = 1;

        if (!pthread_join(positioning, NULL)) {
            printf("Thread positioning joined.\n");
        } else {
            printf("Error joining threads.\n");
        }
        if (!pthread_join(thread, NULL)) {
            printf("Thread receiving joined.\n");
        } else {
            printf("Error joining threads.\n");
        }





    } else {
        fprintf(stderr, "Failed to connect to server...\n");
        sleep(2);
        exit(EXIT_FAILURE);
    }

    close(SOCKET);
    return 0;
}
