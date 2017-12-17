#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include <math.h>


#define Sleep( msec ) usleep(( msec ) * 1000 )

float error=0,correction=0;
float value;
float result;
int maxmedium;
float initial_ang,min_dist;

uint8_t sn[2];
FLAGS_T state;
float value;

uint8_t sn_sonar;
uint8_t sn_gyr;

int max_speed;
int maxmedium;
int i;

char s[ 256 ];

int init() {

  ev3_sensor_init();


  if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0) == 0){
    printf("SONAR missing\n");
    while (!get_sensor_value0(sn_sonar, &value)){
    }

    if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_gyr,0) == 0){
      printf("Gyro missing\n");
      while (!get_sensor_value0(sn_sonar, &value)){
      }

      return 0;
    }

  }}



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

  /*
  void search (uint8_t ss, int max_speed,float angle)
  {
  printf("Search Function");
  float res;
  float dist;

  if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
  {
  get_sensor_value0(sn_gyr,&res);
  printf("Found the Gyro\n");
}

if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0))
{
printf("SONAR found, reading sonar...\n");
if ( !get_sensor_value0(sn_sonar, &dist ))
{
dist = 0;
}
printf( "\r(%f) \n", dist);
fflush( stdout );
}
else
{
printf("SONAR not found\n");
}
result=res;
set_tacho_command_inx( ss, TACHO_RESET );
set_tacho_stop_action_inx( ss, TACHO_COAST );
set_tacho_speed_sp( ss, max_speed * 1 / 12 );
//set_tacho_time_sp( s2, 1400 );
set_tacho_ramp_up_sp( ss, 0 );
set_tacho_ramp_down_sp( ss, 0 );
set_tacho_command_inx( ss, TACHO_RUN_FOREVER );
Sleep(100);
//do {
//  	get_tacho_state_flags( s2, &state );
//  } while ( state );

int b;
if(sn[0]==ss) b=-1;
else b=1;
float min=1000;
float min_ang=res;
while(b*result < b*(res+b*(angle-error)))
{
if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
{
get_sensor_value0(sn_gyr,&result);
printf( "  angle main turn search value %f\n",result);
}
fflush( stdout );
if ( !get_sensor_value0(sn_sonar, &dist ))
{
dist = 0;
}
printf( "distance seen : (%f) \n", dist);
if(dist!=0 && min>dist)
{
min=dist;
min_ang=result;


}

}
min_dist=min;
set_tacho_command_inx( ss, TACHO_STOP);
printf("\n");
printf( "************(%f) *********\n", dist);
Sleep(1000);
set_tacho_speed_sp( ss, -1*max_speed * 1 / 12 );
set_tacho_command_inx( ss, TACHO_RUN_FOREVER );
get_sensor_value0(sn_gyr,&res);
while(b*result >b*(min_ang))
{
if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
{
get_sensor_value0(sn_gyr,&result);
//printf( "  value %f\n",result);
}
fflush( stdout );
if ( !get_sensor_value0(sn_sonar, &dist ))
{
dist = 0;
}
//printf( "\r(%f) \n", dist);
}

set_tacho_command_inx( ss, TACHO_STOP);

Sleep(1000);

while(b*result < b*(min_ang-correction))
{
if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
{
get_sensor_value0(sn_gyr,&result);
//printf( "  value %f\n",result);
}
fflush( stdout );

set_tacho_command_inx( ss, TACHO_RESET );
set_tacho_stop_action_inx( ss, TACHO_COAST );
set_tacho_speed_sp( ss, max_speed * 1 / 12 );
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

while(b*result > b*(min_ang-correction))
{
if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
{
get_sensor_value0(sn_gyr,&result);
//printf( "  value %f\n",result);
}
fflush( stdout );

set_tacho_command_inx( ss, TACHO_RESET );
set_tacho_stop_action_inx( ss, TACHO_COAST );
set_tacho_speed_sp( ss, -1*max_speed * 1 / 12 );
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


printf("--------------------------------------------search END----------------------------\n");

}
*/

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
/*
  if(a==1)
  {
    while(b*result < b*(initial_ang+b*(angle-3)))
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

    }

    while(b*result > b*(initial_ang+b*(angle-3)))
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
      Sleep(500);
    }

  }
  else
  {
    while(b*result > b*(initial_ang-b*(angle-3)))
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

    }
/*
    while(b*result < b*(initial_ang-b*(angle-3)))
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

    } */
  }




int main (void){

  init();
  #ifndef __ARM_ARCH_4T__
  /* Disable auto-detection of the brick (you have to set the correct address below) */
  ev3_brick_addr = "192.168.0.204";

  #endif
  if ( ev3_init() == -1 ) return ( 1 );

  #ifndef __ARM_ARCH_4T__
  printf( "The EV3 brick auto-detection is DISABLED,\nwaiting %s online with plugged tacho...\n", ev3_brick_addr );

  #else
  printf( "Waiting tacho is plugged...\n" );

  #endif
  while ( ev3_tacho_init() < 1 ) Sleep( 1000 );
  ev3_sensor_init();
  printf( "*** ( EV3 ) Hello! ***\n" );

  printf( "Found tacho motors:\n" );
  for ( i = 0; i < DESC_LIMIT; i++ )
  {
    if ( ev3_tacho[ i ].type_inx != TACHO_TYPE__NONE_ )
    {
      printf( "  type = %s\n", ev3_tacho_type( ev3_tacho[ i ].type_inx ));
      printf( "  port = %s\n", ev3_tacho_port_name( i, s ));
      printf("  port = %d %d\n", ev3_tacho_desc_port(i), ev3_tacho_desc_extport(i));
    }
  }


  //Run motors in order from port A to D
  int port1=65,port2=66,port3=68;
  uint8_t s1,s2;
  /*if ( ev3_search_tacho_plugged_in(port1,0, &snmedium, 0 ){
  set_tacho_speed_sp( snmedium, -1* max_speed * 1 / 5 );
  }*/
  if ( ev3_search_tacho_plugged_in(port1,0, &s1, 0 ) && ev3_search_tacho_plugged_in(port2,0, &s2, 0 ))
  {
    int max_speed;
    sn[0]=s1;
    sn[1]=s2;
    if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
    {
      get_sensor_value0(sn_gyr,&initial_ang);
      printf( "  value initial angle: %f\n",initial_ang);
    }
    fflush( stdout );
    multi_set_tacho_command_inx( sn, TACHO_RESET );
    printf( "2 LEGO_EV3_M_MOTORs  are found, run for 5 sec...\n" );
    get_tacho_max_speed( sn[0], &max_speed );
    printf("  max speed = %d\n", max_speed );

    //FIRST PROGRAM
    //********go near first obstacle****************
    Sleep(2000);
    continue_until(max_speed,300);
    Sleep(1000);
    //********first turn to the right****************
    //    turn_absolute(s1,max_speed,1,90.0);
    //    Sleep(1000);
    float dist;
    get_sensor_value0(sn_sonar, &dist );
    printf("Cas demande %f\n",dist);

    while(1)
    {
      if(dist<300)
      {
        printf("Cas demande\n" );
        get_sensor_value0(sn_sonar, &dist );
        printf("Cas demande %f\n",dist);
        turn_absolute(s2,max_speed,1,90.0);

        Sleep(1000);

      }
      continue_until(max_speed,300);
        printf(" NATTTTT %f\n",dist);
    }



    //Sleep(1000);
    //continue_until(max_speed,100);
    //Sleep(2000);

  }
}
/*

//********go to the second one****************
continue_until(max_speed,300);
Sleep(1000);
//********turn ***************
turn_relative(s1,max_speed,1,90.0);
Sleep(1000);
//********go forward a bit more ********************
go_straight(1869,max_speed,1);
Sleep(1000);
//*********calculate initial position******
float ang1,ang2,dist;
if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0)) {
get_sensor_value0(sn_gyr,&ang1);
printf( "  value %f\n",ang1);
}
fflush( stdout );

}
*/
