#include "gsyst.h"

void go_straight(int time,int max_speed, int a)
{
    multi_set_tacho_stop_action_inx( sn, TACHO_COAST );
    multi_set_tacho_speed_sp( sn, a * max_speed * 1 / 4 );
    multi_set_tacho_time_sp( sn, time );
    multi_set_tacho_ramp_up_sp( sn, 0);
    multi_set_tacho_ramp_down_sp( sn, 0 );
    multi_set_tacho_command_inx( sn, TACHO_RUN_TIMED );
    /* Wait tacho stop */
    Sleep(300);
    do
    {
        get_tacho_state_flags( sn[0], &state );
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
    multi_set_tacho_command_inx( sn, TACHO_RESET );
    get_tacho_max_speed( sn[0], &max_speed );
    multi_set_tacho_stop_action_inx( sn, TACHO_COAST );
    multi_set_tacho_speed_sp( sn, max_speed * 1 / 5 );
    //multi_set_tacho_time_sp( sn, 500 );
    multi_set_tacho_ramp_up_sp( sn, 0 );
    multi_set_tacho_ramp_down_sp( sn, 0 );
    multi_set_tacho_command_inx( sn, TACHO_RUN_FOREVER );
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
    multi_set_tacho_command_inx( sn, TACHO_STOP );
}

void turn_relative(uint8_t ss, int max_speed, int a,float angle)
{
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
    if(sn[0]==ss) b=-1;
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
    if(sn[0]==ss) b=-1;
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
