#include "servercom.h"

/*
void debug(const char *fmt, ...) {
    va_list argp;
    va_start(argp, fmt);
    vprintf(fmt, argp);
    va_end(argp);
}
*/

int s;   /* socket */
uint16_t msgId = 0;  /* seq num */

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

int parse_message() {
    char string[58];
    int msg;
    uint8_t dst;
    uint8_t src;
    uint8_t msgType;
    int16_t id_ack;
    uint8_t kick_id;

    msg = read_from_server(s, string, 58);
    
    if (msg > 0) {
        dst = (uint8_t)string[3];
        msgType = (uint8_t)string[4];
    } else {
        fprintf(stderr, "Receiving error\n");
        return -1;
    }

    if (dst != TEAM_ID) {
        printf("Received message for someone else. Ignore!\n");
        return -1;
    }

    switch(msgType) {
        case MSG_ACK:
            id_ack = (int16_t)string[5];
            printf("Received ACK for message %d\n", id_ack);
            break;
        case MSG_START:
            printf("START signal received!\n");
            /* call start function? */
            break;
        case MSG_STOP:
            printf("STOP signal received!\n");
            /* call stop function? */
            break;
        case MSG_KICK:
            kick_id = (uint8_t)string[5];
            printf("Robot with id=%d was kicked by server!\n", kick_id);
            if (kick_id == TEAM_ID) {
                printf("We got kicked!\n");
            }
            /* check if we got kicked? */
            break;
        case MSG_POSITION:
            /* I don't think we should do anything here */
            break;
        

        default:
            printf("Invalid message type!\n");
            return -1;
    }

    return msg;
}

/* Header address of server is 0xFF */
int connect_to_server(){
    struct sockaddr_rc addr = {0};
    int status;

    /* allocate a socket */
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    
    /* set the connection parameters */
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba(SERV_ADDR, &addr.rc_bdaddr);
    
    /* connect to server */
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    return status;
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


