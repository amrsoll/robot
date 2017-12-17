#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//include EV3 libs
#ifndef EV3
#define EV3
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#endif

#ifndef GSYST_H
#define GSYST_H
/* do not include any other librairies in this file */
void continue_until(int,float);

void turn_relative(uint8_t,int,int,float);

void turn_absolute(uint8_t,int,int,float);
/* add the signature of any new functions from the .c file here */

#endif
