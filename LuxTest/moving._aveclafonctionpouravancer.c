#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"


#define Sleep( msec ) usleep(( msec ) * 1000 )

uint8_t sn[2];
//FLAGS_T state;
float value;
uint8_t sn_sonar;
uint8_t sn_gyro;
int max_speed;
/*
int init() {


  ev3_sensor_init();

  if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0) == 0){
      printf("SONAR missing\n");
      while (!get_sensor_value0(sn_sonar, &value)){
      }
      return 0;
  }

}
*/
void Gostraight(int time,int max_speed, uint8_t* sn)
{
    FLAGS_T state;
    multi_set_tacho_stop_action_inx( sn, TACHO_COAST );
    multi_set_tacho_speed_sp( sn, max_speed * 1 / 4 );
    multi_set_tacho_time_sp( sn, time );
    multi_set_tacho_ramp_up_sp( sn, 2000 );
    multi_set_tacho_ramp_down_sp( sn, 2000 );
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
  value=2000.0;
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

if(value<=goal) return;
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
    while(value>goal)
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

void search ()
{
  printf("Search Function");
  float res;
  float dist;

  if ( ev3_search_sensor( LEGO_EV3_GYRO, &sn_gyr ,0))
    {
        get_sensor_value0(sn_gyr,&res);
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


}
int main (void){

  int i;
    uint8_t s;
    // FLAGS_T state;
    char str[ 256 ];

    if ( ev3_init() == -1 ) return ( 1 );

    while ( ev3_tacho_init() < 1 ) Sleep( 1000 );

    int port=65;
    for (port=65; port<69; port++){
      if ( ev3_search_tacho_plugged_in(port,0, &s, 0 )) {
        int max_speed;
        if (s > 0) {
        printf("Detected ID: %d\n", s);
        if (!(sn[0] > 0)) {
          printf("FIrst ID: %d\n", s);
          sn[0] = s;
        } else {
          printf("Second ID: %d\n", s);
          sn[1] = s;
          break;
        }
      }
    }
  }
        printf("Hello: %d and %d\n", sn[0], sn[1]);
        //  init();
      //  Gostraight(5000, 1000, sn);
        search();
        //continue_until(1000, 100);

      //  printf("ANOTHER funtion is going to control the motors\n");
}
