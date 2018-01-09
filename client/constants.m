/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   08/01/2018
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   amrsoll
 * @Last modified time: 09/01/2018
 */

#include <math.h>

#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"

#ifndef CONSTANTS
#define CONSTANTS
//math
#define pi 3.14159265
#define SQRT2 (float)sqrt(2)
#define max(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })
#define min(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })


/* physical values for the robot */
#define ROBOT_RADIUS 15 //in cm
#define MOV_MOTORS_MAX_SPEED 1050
#define ROBOT_SPEED_INCREMENT 7 // value between 1 and 9.
#define ULTRASONIC_SENSOR_PRECISION 15 //in mm, like the ultrasonic sensor return values
#define LEFT_MOTOR_PORT 68
#define RIGHT_MOTOR_PORT 65
#define GRABBING_MOTOR_PORT 66
#define GRAB_SPEED_INCREMENT 500 //completely arbitrary
#define PINCER_CLOSED 0
#define PINCER_OPENED 1
int pincer_state;
#define DISTANCE_DETECT_MOVABLE 150
#define DISTANCE_BEFORE_STOP 200

//positioning
#define ANGLE_BUFFER_SIZE 3
#define ANGLE_BUFFER_LATENCY 10 //duration of sleep between each measure (ms)
#define COMPASS_BUFFER_SIZE 4
#define COMPASS_BUFFER_LATENCY 10
#define POSITION_MESSAGE_DELAY .3 //delay between each position message to the server
float x;
float y;
float init_angle;
float angle; // always : angle = get_angle() - init_angle;
float distance;
//float distance; //measured distance of the sonar (in mm)

// Mapping
#define PIXEL_SIZE 5 //Length of a pixel corner in cm as treated by the robot.
#define PIXEL_SIZE_TO_SERVER 5 //Length of a pixel corner in cm when sent to the server
#define MAP_PATH "map/pixels"
#define MAP_WIDTH 500 // the real width is MAP_WIDTH*PIXEL_SIZE cm
// the real width is MAP_WIDTH_TO_SERVER*PIXEL_SIZE_TO_SERVER cm
#define MAP_WIDTH_TO_SERVER MAP_WIDTH*PIXEL_SIZE/PIXEL_SIZE_TO_SERVER
#define UNDEFINED_PIXEL '+' //pixel type
#define FREE_PIXEL ' ' //pixel type
#define WALL_PIXEL '#' //pixel type
#define SCANNING_MAX_DISTANCE 1500 // in mm, like the ultrasonic sensor return values
#define SCANNING_SPEED 1 // increment of the speed at which the robot turns during mapping scan


// semaphore names
#define LOCK_FOR_READ_NODES "/rnodes"
#define LOCK_FOR_READ_VERTI "/rvertices"
#define LOCK_FOR_WRITE_NODES "/wnodes"
#define LOCK_FOR_WRITE_VERTI "/wvertices"
#define LOCK_AGAINST_STARVATION_NAME "/sem66"

// Miscellanious
#define EXPLORE_ANGLE 45
#define Sleep( msec ) usleep(( msec ) * 1000 )
#define EV3_BRICK_ADDR "192.168.0.204"
char s[ 256 ];
FLAGS_T state;


#endif
