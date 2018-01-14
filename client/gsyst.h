/**
 * @Author: Natalia Balalaeva <nataliabalalaeva>
 * @Date:   08/01/2018
 * @Last modified by:   amrsoll
 * @Last modified time: 14/01/2018
 */



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

uint8_t mov_motors[2]; //motors for the movement
uint8_t grab_motor;
uint8_t sn_sonar;
uint8_t sn_gyr;

float get_angle(void);
float get_distance(void);
int refresh_distance(void);
int refresh_angle(void);
float fmean_value(float*,size_t);
int get_new_coordinates(float,float,float,float);
int init_mov_motors(void);
void start_straight(int);
void start_turn(int);
void stop_mov_motors(void);
int turn_to_angle(float);
float scan_for_obstacle(void);
void continue_until(float);
void turn_absolute(uint8_t,int,float);
int grab(int);

/* add the signature of any new functions from the .c file here */

#endif
