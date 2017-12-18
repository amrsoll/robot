#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"

/* physical values for the robot */
#define ROBOT_RADIUS 25 //completely arbitrary
#define ULTRASONIC_SENSOR_PRECISION 20 //completely arbitrary
#define LEFT_MOTOR_PORT 65
#define RIGHT_MOTOR_PORT 66
#define GRABBING_MOTOR_PORT 67
#define GRAB_SPEED 500 //completely arbitrary
#define PINCER_CLOSED 0
#define PINCER_OPENED 1
int pincer_state;
#define DISTANCE_DETECT_MOVABLE 150


/* semaphore names */
#define LOCK_FOR_READ_NODES "/rnodes"
#define LOCK_FOR_READ_VERTI "/rvertices"
#define LOCK_FOR_WRITE_NODES "/wnodes"
#define LOCK_FOR_WRITE_VERTI "/wvertices"
#define LOCK_AGAINST_STARVATION_NAME "/sem66"

//positioning
#define ANGLE_BUFFER_SIZE 3
#define ANGLE_BUFFER_LATENCY 10 //the duration of sleep between each measure in the buffer
#define COMPASS_BUFFER_SIZE 4
#define COMPASS_BUFFER_LATENCY 10
float x;
float y;
float init_angle;
float angle;


// Miscellanious
#define Sleep( msec ) usleep(( msec ) * 1000 )
#define EV3_BRICK_ADDR "192.168.0.204"
char s[ 256 ];
FLAGS_T state;
