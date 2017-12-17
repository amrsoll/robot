#ifndef SENSORS_H
#define SENSORS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "ev3.h"
#include "ev3_sensor.h"

void sensor_init(uint8_t);
float get_compass_value(uint8_t);
float get_sonar_value(uint8_t);
int get_color_value(uint8_t);

#endif
