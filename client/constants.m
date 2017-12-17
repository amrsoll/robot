/* physical values for the robot */
#define ROBOT_RADIUS 25 //completely arbitrary
#define ULTRASONIC_SENSOR_PRECISION 2 //completely arbitrary
#define LEFT_MOTOR_PORT 65
#define RIGHT_MOTOR_PORT 66
#define GRABBING_MOTOR_PORT 67
#define GRAB_SPEED 500 //completely arbitrary


/* semaphore names */
#define LOCK_FOR_READ_NODES "/rnodes"
#define LOCK_FOR_READ_VERTI "/rvertices"
#define LOCK_FOR_WRITE_NODES "/wnodes"
#define LOCK_FOR_WRITE_VERTI "/wvertices"
#define LOCK_AGAINST_STARVATION_NAME "/sem66"

// Miscellanious
#define Sleep( msec ) usleep(( msec ) * 1000 )
char s[ 256 ];
FLAGS_T state;
