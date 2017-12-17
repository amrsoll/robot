#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"


#define Sleep( msec ) usleep(( msec ) * 1000 )

uint8_t sn[2];
//FLAGS_T state;

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
        printf("state: %d\n", state);
    }
    while ( state );
    printf("state: %d\n. QUIT function.\n", state);
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
        Gostraight(5000, 1000, sn);
        printf("ANOTHER funtion is going to control the motors\n");
}
