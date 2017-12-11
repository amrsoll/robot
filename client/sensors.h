#ifndef SENSORS_H
#define SENSORS_H

void sensor_init(uint8_t);
float get_compass_value(uint8_t);
float get_sonar_value(uint8_t);
int get_color_value(uint8_t);

#endif
