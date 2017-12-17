#include "gsyst.h"



int init_mov_motors() {
    //
    if ( ev3_search_tacho_plugged_in(LEFT_MOTOR_PORT,0, &mov_motors[0], 0 ) )
        printf("is the left motor plugged in? \n" ); //TODO : Is this message printed when the motor is plugged in already?

    if ( ev3_search_tacho_plugged_in(RIGHT_MOTOR_PORT,0, &mov_motors[1],0 ) )
        printf("is the right motor plugged in? \n"); //TODO : Is this message printed when the motor is plugged in already?

    if ( ev3_search_tacho_plugged_in(GRABBING_MOTOR_PORT,0, &grab_motor,0 ) )
        printf("is the grabbing motor plugged in? \n"); //TODO : Is this message printed when the motor is plugged in already?
}

void go_straight(int time,int max_speed, int a)
{
    multi_set_tacho_stop_action_inx( mov_motors, TACHO_COAST );
    multi_set_tacho_speed_sp( mov_motors, a * max_speed * 1 / 4 );
    multi_set_tacho_time_sp( mov_motors, time );
    multi_set_tacho_ramp_up_sp( mov_motors, 0);
    multi_set_tacho_ramp_down_sp( mov_motors, 0 );
    multi_set_tacho_command_inx( mov_motors, TACHO_RUN_TIMED );
    /* Wait tacho stop */
    Sleep(300);
    do
    {
        get_tacho_state_flags( mov_motors[0], &state );
        //  printf("state: %d\n", state);
    }
    while ( state );
    //printf("state: %d\n. QUIT function.\n", state);
}

void continue_until(int max_speed, float goal)
{
    value = 2000.0;
    if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0))
    {
        printf("SONAR found, reading sonar...\n");
        if ( !get_sensor_value0(sn_sonar, &value ))
        {
            value = 0;
        }
        printf( "\r(%f) \n", value);
        fflush( stdout );
    }
    if(value <= goal) return;
    multi_set_tacho_command_inx( mov_motors, TACHO_RESET );
    get_tacho_max_speed( mov_motors[0], &max_speed );
    multi_set_tacho_stop_action_inx( mov_motors, TACHO_COAST );
    multi_set_tacho_speed_sp( mov_motors, max_speed * 1 / 5 );
    //multi_set_tacho_time_sp( mov_motors, 500 );
    multi_set_tacho_ramp_up_sp( mov_motors, 0 );
    multi_set_tacho_ramp_down_sp( mov_motors, 0 );
    multi_set_tacho_command_inx( mov_motors, TACHO_RUN_FOREVER );
    //	Sleep( 300 );
    // sleep(200);
    while(value > goal)
    {
        if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0))
        {
            printf("SONAR found, reading sonar...\n");
            if ( !get_sensor_value0(sn_sonar, &value ))
            {
                value = 0;
            }
            printf( "\r(%f) \n", value);
            fflush( stdout );
        }
    }
    multi_set_tacho_command_inx( mov_motors, TACHO_STOP );
}

void turn_relative(uint8_t ss, int max_speed, int a,float angle)
{
    error = 0;
    correction = 0;
    float res;
    printf("Here we will turn\n");
    if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
    {
        get_sensor_value0(sn_gyr,&res);
    }
    result = res;
    set_tacho_command_inx( ss, TACHO_RESET );
    set_tacho_stop_action_inx( ss, TACHO_COAST );
    set_tacho_speed_sp( ss, a*max_speed * 1 / 10 );
    set_tacho_ramp_up_sp( ss, 0 );
    set_tacho_ramp_down_sp( ss, 0 );
    set_tacho_command_inx( ss, TACHO_RUN_FOREVER );
    Sleep(100);
    int b;
    if(mov_motors[0]==ss) b=-1;
    else b=1;
    if(a==1)
    {
        while(b*result < b*(res+b*(angle-error)))
        {
            if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
            {
                get_sensor_value0(sn_gyr,&result);
                printf( "  value %f\n",result);
            }
            fflush( stdout );

        }
    }
    else
    {
        while(b*result > b*(res-b*(angle-error)))
        {
            if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
            {
                get_sensor_value0(sn_gyr,&result);
                printf( "  value %f\n",result);
            }
            fflush( stdout );
        }
    }
    set_tacho_command_inx( ss, TACHO_STOP);
    Sleep(1000);
    if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
    {
        get_sensor_value0(sn_gyr,&result);
        printf( "  value %f\n",result);
    }
    fflush( stdout );

    if(a==1)
    {
        while(b*result < b*(res+b*(angle-correction)))
        {
            if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
            {
                get_sensor_value0(sn_gyr,&result);
                printf( "  value %f\n",result);
            }
            fflush( stdout );
            set_tacho_command_inx( ss, TACHO_RESET );
            set_tacho_stop_action_inx( ss, TACHO_COAST );
            set_tacho_speed_sp( ss, a*max_speed * 1 / 10 );
            set_tacho_time_sp(ss,100);
            set_tacho_ramp_up_sp( ss, 0 );
            set_tacho_ramp_down_sp( ss, 0 );
            set_tacho_command_inx( ss, TACHO_RUN_TIMED );
            Sleep(1000);
            if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
            {
                get_sensor_value0(sn_gyr,&result);
                printf( "  value %f\n",result);
            }
        }
        while(b*result > b*(res+b*(angle-correction)))
        {
            if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
            {
                get_sensor_value0(sn_gyr,&result);
                //printf( "  value %f\n",result);
            }
            fflush( stdout );
            set_tacho_command_inx( ss, TACHO_RESET );
            set_tacho_stop_action_inx( ss, TACHO_COAST );
            set_tacho_speed_sp( ss, -1*a*max_speed * 1 / 10 );
            set_tacho_time_sp(ss,100);
            set_tacho_ramp_up_sp( ss, 0 );
            set_tacho_ramp_down_sp( ss, 0 );
            set_tacho_command_inx( ss, TACHO_RUN_TIMED );
            Sleep(1000);
            if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
            {
                get_sensor_value0(sn_gyr,&result);
                //printf( "  value %f\n",result);
            }
        }
    }
    else
    {
        while(b*result > b*(res-b*(angle-correction)))
        {
            if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
            {
                get_sensor_value0(sn_gyr,&result);
                //printf( "  value %f\n",result);
            }
            fflush( stdout );
            set_tacho_command_inx( ss, TACHO_RESET );
            set_tacho_stop_action_inx( ss, TACHO_COAST );
            set_tacho_speed_sp( ss, a*max_speed * 1 / 10 );
            set_tacho_time_sp(ss,100);
            set_tacho_ramp_up_sp( ss, 0 );
            set_tacho_ramp_down_sp( ss, 0 );
            set_tacho_command_inx( ss, TACHO_RUN_TIMED );
            Sleep(1000);
            if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
            {
                get_sensor_value0(sn_gyr,&result);
                //printf( "  value %f\n",result);
            }
        }
        while(b*result < b*(res-b*(angle-correction)))
        {
            if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
            {
                get_sensor_value0(sn_gyr,&result);
                //printf( "  value %f\n",result);
            }
            fflush( stdout );
            set_tacho_command_inx( ss, TACHO_RESET );
            set_tacho_stop_action_inx( ss, TACHO_COAST );
            set_tacho_speed_sp( ss, -1*a*max_speed * 1 / 10 );
            set_tacho_time_sp(ss,100);
            set_tacho_ramp_up_sp( ss, 0 );
            set_tacho_ramp_down_sp( ss, 0 );
            set_tacho_command_inx( ss, TACHO_RUN_TIMED );
            Sleep(1000);
            if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
            {
                get_sensor_value0(sn_gyr,&result);
                //printf( "  value %f\n",result);
            }
        }
    }
}
void turn_absolute(uint8_t ss, int max_speed, int a,float angle)
{
    if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
    {
        get_sensor_value0(sn_gyr,&result);
    }
    set_tacho_command_inx( ss, TACHO_RESET );
    set_tacho_stop_action_inx( ss, TACHO_COAST );
    set_tacho_speed_sp( ss, a*max_speed * 1 / 10 );
    set_tacho_ramp_up_sp( ss, 0 );
    set_tacho_ramp_down_sp( ss, 0 );
    set_tacho_command_inx( ss, TACHO_RUN_FOREVER );
    Sleep(100);
    int b;
    if(mov_motors[0]==ss) b=-1;
    else b=1;
    if(a==1){
        while(b*result < b*(initial_ang+b*(angle-5)))
        {
            if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
            {
                get_sensor_value0(sn_gyr,&result);
                //printf( "  value %f\n",result);
                printf( " if case\n");
            }
            fflush( stdout );
        }
    }
    else
    {
        while(b*result > b*(initial_ang-b*(angle-5)))
        {
            if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
            {
                get_sensor_value0(sn_gyr,&result);
                printf( "  value %f\n",result);
                printf( "  else case\n");
            }
            fflush( stdout );
        }
    }
    set_tacho_command_inx( ss, TACHO_STOP);
    Sleep(1000);
    if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
    {
        get_sensor_value0(sn_gyr,&result);
        //printf( "  value %f\n",result);
    }
    fflush( stdout );
}

//////////////  RELEASE THE BALL


int grab()
{
    if ( ev3_search_tacho_plugged_in(GRABBING_MOTOR_PORT,0, &grab_motor, 0 )) {
		int max_speed;

		printf( "LEGO_EV3_M_MOTOR 1 is found, run for 5 sec...\n" );
		get_tacho_max_speed( grab_motor, &max_speed );
		printf("  max speed = %d\n", max_speed );
		set_tacho_stop_action_inx( grab_motor, TACHO_COAST );
		set_tacho_speed_sp( grab_motor, -max_speed * 1 / 5 );
		set_tacho_time_sp( grab_motor, 500 );
		set_tacho_ramp_up_sp( grab_motor, 2000 );
		set_tacho_ramp_down_sp( grab_motor, 2000 );
		set_tacho_command_inx( grab_motor, TACHO_RUN_TIMED );
	} else {
		printf( "LEGO_EV3_M_MOTOR 1 is NOT found\n" );
	}
}

int release()
{
    // int max_speed;
    // multi_set_tacho_command_inx( grab_motor, TACHO_RESET );
    // get_tacho_max_speed( grab_motor[0], &max_speed );
    // printf("  max speed = %d\n", max_speed );
    // multi_set_tacho_command_inx( grab_motor, TACHO_RESET );
    // get_tacho_max_speed( grab_motor[0], &max_speed );
    // multi_set_tacho_stop_action_inx( grab_motor, TACHO_COAST );
    // multi_set_tacho_speed_sp( grab_motor, max_speed * 1 / 3 );
    // //multi_set_tacho_time_sp( grab_motor, 500 );
    // multi_set_tacho_ramp_up_sp( grab_motor, 0 );
    // multi_set_tacho_ramp_down_sp( grab_motor, 0 );
    // multi_set_tacho_command_inx( grab_motor, TACHO_RUN_FOREVER );
    // sleep(1);
    // multi_set_tacho_command_inx( grab_motor, TACHO_STOP );


	if ( ev3_search_tacho_plugged_in(GRABBING_MOTOR_PORT,0, &grab_motor, 0 )) {
		int max_speed;

		printf( "LEGO_EV3_M_MOTOR 1 is found, run for 5 sec...\n" );
		get_tacho_max_speed( grab_motor, &max_speed );
		printf("  max speed = %d\n", max_speed );
		set_tacho_stop_action_inx( grab_motor, TACHO_COAST );
		set_tacho_speed_sp( grab_motor, max_speed * 1 / 5 );
		set_tacho_time_sp( grab_motor, 500 );
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
		// set_tacho_speed_sp( grab_motor, max_speed / 2 );
		// set_tacho_ramp_up_sp( grab_motor, 0 );
		// set_tacho_ramp_down_sp( grab_motor, 0 );
		// set_tacho_position_sp( grab_motor, 90 );
		// for ( i = 0; i < 8; i++ ) {
		// 	set_tacho_command_inx( grab_motor, TACHO_RUN_TO_REL_POS );
		// 	Sleep( 500 );
		// }
	} else {
		printf( "LEGO_EV3_M_MOTOR 1 is NOT found\n" );
	}
}
