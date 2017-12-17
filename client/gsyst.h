#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//include EV3 library
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"

//local library
#include "constants.m"

#ifndef GSYST_H
#define GSYST_H

float error, correction;
float value;
float result;
int maxmedium;
float initial_ang,min_dist;
uint8_t mov_motors[2]; //motors for the movement
uint8_t grab_motor;
uint8_t sn_sonar;
uint8_t sn_gyr;
int max_speed;
int maxmedium;
int i;
char s[ 256 ];

void continue_until(int,float);

void turn_relative(uint8_t,int,int,float);

void turn_absolute(uint8_t,int,int,float);

int grab(void);

int release(void);
/* add the signature of any new functions from the .c file here */

#endif
