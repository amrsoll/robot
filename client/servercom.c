/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   13/01/2018
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   amrsoll
 * @Last modified time: 14/01/2018
 */



#include "servercom.h"

uint16_t msgId = 0; /* msg seq num */

ssize_t read_from_server(int sock, char *msg, size_t maxSize) {
    printf("meh\n");
    ssize_t bytes_read = read(sock, msg, maxSize);
    printf("meh?\n");
    if (bytes_read <= 0) {
        fprintf(stderr, "Server unexpectedly closed connection...\n");
        close(sock);
        exit(EXIT_FAILURE);
    }
    printf("[DEBUG] received %d bytes\n", bytes_read);
    return bytes_read;
}

int send_to_server(int sock, char *data, size_t size) {
    return write(sock, data, size);
}

int parse_message(int sock) {
    char string[58];
    int bytes_read;
    uint8_t dst;
    uint8_t src;
    uint8_t msgType;
    int16_t id_ack;
    uint8_t kick_id;
    bytes_read = read_from_server(sock, string, 58);
    if (bytes_read > 0) {
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
    return bytes_read;
}

/* Messages sent by the server should not be acknowledged */
int send_ACK(int sock, uint8_t dst, int16_t id_ack, int8_t state){
    char string[8];
    *((uint16_t *) string) = msgId++;
    string[2] = TEAM_ID;
    string[3] = dst;
    string[4] = MSG_ACK;
    string[5] = id_ack;
    string[6] = 0x00;
    string[7] = state;
    return send_to_server(sock, string, 8);
}

/* must be sent every two seconds */
int send_POSITION(int sock, int16_t x, int16_t y){
    char string[9];
    *((uint16_t *) string) = msgId++;
    string[2] = TEAM_ID;
    string[3] = 0xFF;
    string[4] = MSG_POSITION;
    string[5] = x;
    string[6] = 0x00;
    string[7] = y;
    string[8] = 0x00;
    return send_to_server(sock, string, 9);
}

/* After the entire map has been generated, the robot */
/* sends the server each 5x5 cm grid one pixel at a time */
int send_MAPDATA(int sock, int16_t x, int16_t y, uint8_t R, uint8_t G, uint8_t B){
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
    return send_to_server(sock, string, 12);
}

/* These messages signal to the server that their map is finished */
int send_MAPDONE(int sock) {
    char string[5];
    *((uint16_t *) string) = msgId++;
    string[2] = TEAM_ID;
    string[3] = 0xFF;
    string[4] = MSG_MAPDONE;
    return send_to_server(sock, string, 5);
}

/* OBSTACLE messages must be sent when a robot picks up or drop an obstacle */
int send_OBSTACLE(int sock, uint8_t act, int16_t x, int16_t y){
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
    return send_to_server(sock, string, 10);
}
