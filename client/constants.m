/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   08/01/2018
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   madafaka
 * @Last modified time: 15/01/2018
 */

#include <math.h>

#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "classes.h"

#ifndef CONSTANTS
#define CONSTANTS
//math
#define pi 3.14159265
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
#define FULL_TURN_ANGLE 360.0

//sensors
#define GYRO_SAMPLE_RATE 1000 // in HZ // Sample rate of 1 kHz

//positioning
#define ANGLE_BUFFER_SIZE 3
#define ANGLE_BUFFER_LATENCY 1000/GYRO_SAMPLE_RATE+1 //duration of sleep between each measure (ms)
#define COMPASS_BUFFER_SIZE 4
#define COMPASS_BUFFER_LATENCY 10
#define POSITION_MESSAGE_DELAY .3 //delay between each position message to the server
fPoint robotPosition;
tCoord start_position;
float init_angle;
float angle; // always : angle = get_angle() - init_angle;
float distance; //measured distance of the sonar (in mm)
float init_distance;

// Mapping
#define PIXEL_SIZE 2 //Length of a pixel corner in cm as treated by the robot.
#define PIXEL_SIZE_TO_SERVER 5 //Length of a pixel corner in cm when sent to the server
#define MAP_PATH "map/pixels"
#define MAP_WIDTH 500 // the real width is MAP_WIDTH*PIXEL_SIZE cm
// the real width is MAP_WIDTH_TO_SERVER*PIXEL_SIZE_TO_SERVER cm
#define MAP_WIDTH_TO_SERVER MAP_WIDTH*PIXEL_SIZE/PIXEL_SIZE_TO_SERVER
#define UNDEFINED_PIXEL '+' //pixel type
#define FREE_PIXEL ' ' //pixel type
#define WALL_PIXEL '#' //pixel type
#define SCANNING_MAX_DISTANCE 1500.0 // in mm, like the ultrasonic sensor return values
#define SCANNING_SPEED 1 // increment of the speed at which the robot turns during mapping scan
#define MM_TO_PIX_SIZE (float)1/(10*PIXEL_SIZE)
#define MM_TO_PIX_SIZE_TO_SERVER (float)1/(10*PIXEL_SIZE_TO_SERVER)

//Pathing
#define PATH_PATH "~/map/path"

// semaphore names
#define LOCK_FOR_READ_NODES "/rnodes"
#define LOCK_FOR_READ_VERTI "/rvertices"
#define LOCK_FOR_WRITE_NODES "/wnodes"
#define LOCK_FOR_WRITE_VERTI "/wvertices"
#define LOCK_AGAINST_STARVATION_NAME "/sem66"

// Miscellanious
#define EXPLORE_ANGLE 45
#define Sleep( msec ) usleep(( msec ) * 1000 )
FLAGS_T state;


#endif
