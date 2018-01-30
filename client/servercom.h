/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   13/01/2018
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   amrsoll
 * @Last modified time: 14/01/2018
 */



#ifndef SERVERCOM_H
#define SERVERCOM_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <sys/socket.h>
#include <math.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <sys/types.h>
#include <pthread.h>

#define SERV_ADDR "00:19:0E:10:72:CB" //This is my address(Axel) //"00:1A:7D:DA:71:06"
#define EV3_BRICK_ADDR "192.168.0.204"
/* ZEN = 60:57:18:3e:03:86 */
/* OS server = 00:1A:7D:DA:71:06 */

#define TEAM_ID 9

#define MSG_ACK 0
#define MSG_START 1
#define MSG_STOP 2
#define MSG_KICK 3
#define MSG_POSITION 4
#define MSG_MAPDATA 5
#define MSG_MAPDONE 6
#define MSG_OBSTACLE 7
//#define SOCKET socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)


//void debug( char*, ...);
int read_from_server(int, char*, size_t);
int send_to_server(int, char*, size_t);
int parse_message(int);
int send_ACK(int, uint8_t, int16_t, int8_t);
int send_POSITION(int, int16_t, int16_t);
int send_MAPDATA(int, int16_t, int16_t, uint8_t, uint8_t, uint8_t);
int send_MAPDONE(int);
int send_OBSTACLE(int, uint8_t, int16_t, int16_t);


#endif
