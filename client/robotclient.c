//#include "gsyst.h"
//#include "map.h"
//#include "path.h"
//#include "spot.h"
#include "servercom.h"

int s; /* socket */
uint16_t msgId = 0; /* msg seq num */

int read_from_server(int sock, char *buffer, size_t maxSize) {
    int bytes_read = read(sock, buffer, maxSize);

    if (bytes_read <= 0) {
        fprintf(stderr, "Server unexpectedly closed connection...\n");
        close(s);
        exit(EXIT_FAILURE);
    }

    printf("[DEBUG] received %d bytes\n", bytes_read);

    return bytes_read;
}

int send_to_server(char *data, size_t size) {
    return write(s, data, size);
}

/* Messages sent by the server should not be acknowledged */
int send_ACK(uint8_t dst, int16_t id_ack, int8_t state){
    char string[8];

    *((uint16_t *) string) = msgId++;
    string[2] = TEAM_ID;
    string[3] = dst;
    string[4] = MSG_ACK;
    string[5] = id_ack;
    string[6] = 0x00;
    string[7] = state;

    return send_to_server(string, 8);

}

/* must be sent every two seconds */
int send_POSITION(int16_t x, int16_t y){
    char string[9];

    *((uint16_t *) string) = msgId++;
    string[2] = TEAM_ID;
    string[3] = 0xFF;
    string[4] = MSG_POSITION;
    string[5] = x;
    string[6] = 0x00;
    string[7] = y;
    string[8] = 0x00;

    return send_to_server(string, 9);
}

/* After the entire map has been generated, the robot */ 
/* sends the server each 5x5 cm grid one pixel at a time */
int send_MAPDATA(int16_t x, int16_t y, uint8_t R, uint8_t G, uint8_t B){
    char string[12];

    *((uint16_t *) string) = msgId++;
    string[2] = TEAM_ID;
    string[3] = 0xFF;
    string[4] = MSG_MAPDATA;
    string[5] = x; 
    string[6] = 0x00;
    string[7] = y;
    string[8] = 0x00;
    string[9] = R; // red value of pixel
    string[10] = G; // green value of pixel
    string[11] = B; // blue value of pixel

    return send_to_server(string, 12);

}

/* These messages signal to the server that their map is finished */
int send_MAPDONE() {
    char string[5];

    *((uint16_t *) string) = msgId++;
    string[2] = TEAM_ID;
    string[3] = 0xFF;
    string[4] = MSG_MAPDONE;
    
    return send_to_server(string, 5);
}

/* OBSTACLE messages must be sent when a robot picks up or drop an obstacle */
int send_OBSTACLE(uint8_t act, int16_t x, int16_t y){
    char string[10];

    *((uint16_t *) string) = msgId++;
    string[2] = TEAM_ID;
    string[3] = 0xFF;
    string[4] = MSG_OBSTACLE;
    string[5] = act;  // 0 = dropped, 1 = picked up
    string[6] = x;
    string[7] = 0x00;
    string[8] = y;
    string[9] = 0x00;

    return send_to_server(string, 10);
}


int main(int argc, char **argv) {
    
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
        posX=100;
        posY=100;

        for (i=0;i<30;i++) {
            send_POSITION(posX, posY);
            posX+=1;
            posY-=1;
            Sleep(1000);
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
