/**
 * @Author: Natalia Balalaeva <nataliabalalaeva>
 * @Date:   08/01/2018
 * @Last modified by:   amrsoll
 * @Last modified time: 09/01/2018
 */



#include "gsyst.h"

float get_angle()
{
    float output_angle = 0.0;
    //float angle_buffer[ANGLE_BUFFER_SIZE];
    float measured_angle;
    int i;
    for(i=0; i<ANGLE_BUFFER_SIZE; i=i+1)
        if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
        {
            get_sensor_value0(sn_gyr,&measured_angle);
            output_angle = output_angle + measured_angle;
            //printf("measuring angle number %d, it's value is %f\n",i, measured_angle);
            Sleep(ANGLE_BUFFER_LATENCY);
            fflush( stdout );
        }
    output_angle = output_angle/ANGLE_BUFFER_SIZE;
    return output_angle;
}

float get_distance()
{
    float output;
    if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0))
    {
        get_sensor_value0(sn_sonar, &output );
            printf("Sonar : %f\n", output);
        fflush( stdout );
        return output;
    } else
    {
        printf("failed to connect to the sonar\n");
        return -1.0;
    }
}

int refresh_distance()
//refreshes the global variable distance
{
    if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0))
    {
        get_sensor_value0(sn_sonar, &distance );
        fflush( stdout );
        return 0;
    } else
    {
        printf("failed to connect to the sonar\n");
        return -1;
    }
}

int refresh_angle()
//refreshes the global variable angle
{
    get_sensor_value0(sn_gyr,&angle);
    Sleep(ANGLE_BUFFER_LATENCY);
    fflush( stdout );
    //float angle_buffer[ANGLE_BUFFER_SIZE];
    float measured_angle;
    int i;
    if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
    {
        for(i=1; i<ANGLE_BUFFER_SIZE; i=i+1)
        {
            get_sensor_value0(sn_gyr,&measured_angle);
            angle = angle + measured_angle;
            //printf("measuring angle number %d, it's value is %f\n",i, measured_angle);
            Sleep(ANGLE_BUFFER_LATENCY);
            fflush( stdout );
        }
        angle = angle - init_angle;
        return 0;
    } else{
        return -1;
    }
}

int get_new_coordinates(float x0, float y0,float dist, float current_angle)
{
    float coord[2] = {x0+dist*sin(current_angle/pi),
                      y0+dist*cos(current_angle/pi)};
    x = coord[0];
    y = coord[1];
    return 0;
}

int init_mov_motors() {
    //
    if ( !ev3_search_tacho_plugged_in(LEFT_MOTOR_PORT,0, &mov_motors[0], 0 ) )
        printf("is the left motor plugged in? \n" ); //TODO : Is this message printed when the motor is plugged in already?

    if ( !ev3_search_tacho_plugged_in(RIGHT_MOTOR_PORT,0, &mov_motors[1],0 ) )
        printf("is the right motor plugged in? \n"); //TODO : Is this message printed when the motor is plugged in already?

    if ( !ev3_search_tacho_plugged_in(GRABBING_MOTOR_PORT,0, &grab_motor,0 ) )
        printf("is the grabbing motor plugged in? \n"); //TODO : Is this message printed when the motor is plugged in already?
}

void start_straight(int a)
// a positive : move forward   0 < a <  9 depending on speed
// a negative : move backward  0 > a > -9 depending on speed
{
    multi_set_tacho_command_inx( mov_motors, TACHO_RESET );
    multi_set_tacho_stop_action_inx( mov_motors, TACHO_COAST );
    multi_set_tacho_speed_sp(mov_motors, a*MOV_MOTORS_MAX_SPEED * 1 / 11 );
    multi_set_tacho_ramp_up_sp( mov_motors, 0 );
    multi_set_tacho_ramp_down_sp( mov_motors, 0 );
    multi_set_tacho_command_inx( mov_motors, TACHO_RUN_FOREVER );
    //fflush( stdout );
}

void start_turn(int a)
// a positive : turn right 0 < a <  9 depending on speed
// a negative : turn left  0 > a > -9 depending on speed
{
    multi_set_tacho_command_inx( mov_motors, TACHO_RESET );
    multi_set_tacho_stop_action_inx( mov_motors, TACHO_COAST );
    set_tacho_speed_sp(mov_motors[0],  a*MOV_MOTORS_MAX_SPEED * 1 / 10 );
    set_tacho_speed_sp(mov_motors[1], -a*MOV_MOTORS_MAX_SPEED * 1 / 10 );
    multi_set_tacho_ramp_up_sp( mov_motors, 0 );
    multi_set_tacho_ramp_down_sp( mov_motors, 0 );
    multi_set_tacho_command_inx( mov_motors, TACHO_RUN_FOREVER );
    //fflush( stdout );
}

void stop_mov_motors()
{
    multi_set_tacho_command_inx( mov_motors, TACHO_STOP);
    //fflush( stdout );
}

int turn_to_angle(float angle)
{
    float current_angle = get_angle();
    int turn_direction;
    if( (int)(current_angle-angle)%360 > 180
    ||0>(int)(current_angle-angle)%360 >-180)
        turn_direction = 1;
    else
        turn_direction =-1;
    //get back to initial position.
    //TODO increment turning speed according to how close both angles are.
    // (slower the closer you get, probably 3 different speeds)
    start_turn(turn_direction);
    while(abs(current_angle-angle) > 5) //TODO check if this is the correct formula
    {
        current_angle = get_angle();
    }
    stop_mov_motors();
    return 0;
}

float scan_for_obstacle()
//will hopefully be obsolete
{
    float init_check_angle = get_angle();
    float check_angle = init_check_angle;
    float closest_obstacle = get_distance();
    float checked_distance;
    //turn around to see how far you can go
    start_turn(1);
    while(abs(check_angle-init_check_angle) < EXPLORE_ANGLE)
        check_angle = get_angle();
    stop_mov_motors();
    //turn back the other way and scan.
    start_turn(-1);
    sleep(100); //wait for the difference to be under the explore angle again
    check_angle = get_angle();
    while(abs(check_angle-init_check_angle) < EXPLORE_ANGLE)
    {
        get_sensor_value0(sn_sonar, &distance );
        checked_distance = distance*cos(check_angle - init_check_angle);
        if(checked_distance < closest_obstacle)
            closest_obstacle = checked_distance;
        check_angle = get_angle();
    }
    stop_mov_motors();
    turn_to_angle(init_check_angle);
}

void continue_until(float goal)
{
    distance = get_distance();
    if(distance <= goal) return;
    float init_distance = distance;
    float init_coord[2] = {x,y};
    angle = get_angle();
    start_straight(ROBOT_SPEED_INCREMENT);
    //	Sleep( 300 );
    // sleep(200);
    int init_time = time(NULL);
    while(distance > goal /*&& abs(distance-init_distance) < closest_obstacle - DISTANCE_BEFORE_STOP*/)
    {
        distance = get_distance();
        angle = get_angle();
        get_new_coordinates(init_coord[0],
                            init_coord[1],
                            distance-init_distance,
                            angle-init_angle);
        int current_time = time(NULL)-init_time;
        if(current_time > POSITION_MESSAGE_DELAY)
        {
            printf("x : %f   ;   y : %f\n", x, y);
            init_time = time(NULL);
        }
    }
    stop_mov_motors();
}

void turn_absolute(uint8_t ss, int a,float angle)
{
    float result;
    int b;
    if(mov_motors[0]==ss)
    {
        b=1;
    }
    else
    {
        b=-1;
    }

    float angle_before_turn = get_angle();
    printf("initial angle : %f\n", angle_before_turn);
    start_turn(b);
    if(a==1){
        printf("next angle : %f \n", b*(angle_before_turn+b*(angle-5)));
        while(b*result < b*(angle_before_turn+b*(angle-5)))
        {
            result = get_angle();
            printf("result : %f\n", result );
        }
    }
    else
    {
        while(b*result > b*(angle_before_turn-b*(angle-5)))
        {
            result = get_angle();
        }
    }
    stop_mov_motors();
    Sleep(1000);
    result = get_angle();
}

//////////////  RELEASE THE BALL

int  grab(int instruct)
{
    ev3_sensor_init();
    // multi_set_tacho_command_inx( grab_motor, TACHO_RESET );
    // multi_set_tacho_stop_action_inx( grab_motor, TACHO_COAST );
    // multi_set_tacho_speed_sp( grab_motor, MOV_MOTORS_MAX_SPEED * 1 / 3 );
    // //multi_set_tacho_time_sp( grab_motor, 500 );
    // multi_set_tacho_ramp_up_sp( grab_motor, 0 );
    // multi_set_tacho_ramp_down_sp( grab_motor, 0 );
    // multi_set_tacho_command_inx( grab_motor, TACHO_RUN_FOREVER );
    // sleep(1);
    // multi_set_tacho_command_inx( grab_motor, TACHO_STOP );
	if ( ev3_search_tacho_plugged_in(GRABBING_MOTOR_PORT,0, &grab_motor, 0 ))
    {
		printf( "LEGO_EV3_M_MOTOR grab motor is found, running...\n" );
		set_tacho_stop_action_inx( grab_motor, TACHO_COAST );
		set_tacho_speed_sp( grab_motor, instruct*MOV_MOTORS_MAX_SPEED * 1 / 5 );
		set_tacho_time_sp( grab_motor, 800 );
		set_tacho_ramp_up_sp( grab_motor, 2000 );
		set_tacho_ramp_down_sp( grab_motor, 2000 );
		set_tacho_command_inx( grab_motor, TACHO_RUN_TIMED );
                ///////////   RUN TO REAL POSITION

		// /* Wait tacho stop */
		// Sleep( 100 );
		// do {
		// 	get_tacho_state_flags( grab_motor, &state );
		// } while ( state );
		// printf( "run to relative position...\n" );
		// set_tacho_speed_sp( grab_motor, MOV_MOTORS_MAX_SPEED / 2 );
		// set_tacho_ramp_up_sp( grab_motor, 0 );
		// set_tacho_ramp_down_sp( grab_motor, 0 );
		// set_tacho_position_sp( grab_motor, 90 );
		// for ( i = 0; i < 8; i++ ) {
		// 	set_tacho_command_inx( grab_motor, TACHO_RUN_TO_REL_POS );
		// 	Sleep( 500 );
		// }

	} else {
		printf( "LEGO_EV3_M_MOTOR grab motor is NOT found\n" );
	}
}
