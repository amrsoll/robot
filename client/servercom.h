#ifndef SERVERCOM_H
#define SERVERCOM_H

void debug(const char*, ...);
int read_from_server(int, char*, size_t);
int get_message();
int connect_to_server();
int send_to_server(char*, size_t);
int send_ACK(uint8_t, int16_t, int8_t);
int send_POSITION(int16_t, int16_t);
int send_MAPDATA(int16_t, int16_t, uint8_t, uint8_t, uint8_t);
int send_MAPDONE();
int send_OBSTACLE(uint8_t, int16_t, int16_t);

#endif
