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

#define SERV_ADDR "00:19:0E:10:72:CB" //This is my address(Axel) //"00:1A:7D:DA:71:06"
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

#define Sleep(msec) usleep ((msec)*1000)


//void debug(const char*, ...);
int read_from_server(int, char*, size_t);
int send_to_server(char*, size_t);
int parse_message(void);
int send_ACK(uint8_t, int16_t, int8_t);
int send_POSITION(int16_t, int16_t);
int send_MAPDATA(int16_t, int16_t, uint8_t, uint8_t, uint8_t);
int send_MAPDONE(void);
int send_OBSTACLE(uint8_t, int16_t, int16_t);


#endif
