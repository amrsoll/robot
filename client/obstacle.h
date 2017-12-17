#ifndef obstacle_h
#define obstacle_h

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"

#include "sensors.h"

int detect_obstacle(uint8_t, uint8_t, int);

#endif
