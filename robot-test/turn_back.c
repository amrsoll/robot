#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "turn_back.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "ev3_port.h"


#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

float init_sonar;

void turn_back(uint8_t sn_sonar,
	uint8_t sensor_left,
	uint8_t sensor_right,
	uint8_t motor[2],
	float value,
	float real_sonar_val,
	float sonar_abs,
	float distance)
{

	float init_sonar;
	float measured_sonar_val;
	int maximum_robot_speed;
	int position;
	int save_curr_position;
        //printf("Demi tour...\n" );
        init_sonar = init_sonar - 180;

        if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_sonar,0)){

                if ( !get_sensor_value0(sn_sonar, &value )) {

                        value = init_sonar - 180;

                }

               real_sonar_val = measured_sonar_val-init_sonar;
			sonar_abs = abs(real_sonar_val);


        }

        while (sonar_abs > 2.0) {

                //motors stop
                set_tacho_command_inx( sensor_left, TACHO_STOP );

                set_tacho_command_inx( sensor_right, TACHO_STOP );
                if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_sonar,0)){

                        if ( !get_sensor_value0(sn_sonar, &value )) {

                                value = init_sonar + 90;

                        }

                        real_sonar_val = measured_sonar_val-init_sonar;
					sonar_abs = abs(real_sonar_val);

                        //printf( "\r(%f) \n", real_sonar_val);
                        //fflush( stdout );

                }
                        set_tacho_position_sp( sensor_right, real_sonar_val );
                        set_tacho_position_sp( sensor_left, -real_sonar_val );
                        multi_set_tacho_command_inx( motor, TACHO_RUN_TO_REL_POS );
                        //Sleep(gyro_abs*5);
                      //  do {
                             //   get_tacho_state_flags( sensor_right, &state );
                       // } while (state);

        }

}
