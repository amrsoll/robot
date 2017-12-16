#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "turn_function_left.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "ev3_port.h"


#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

float init_sonar;

void turn_function_left(uint8_t sn_sonar,
	uint8_t sensor_left,
	uint8_t sensor_right,
	uint8_t motor[2],
	float init_sonar,
	float distance)
{

        init_sonar = init_sonar - 90;
  printf("I will turn left...\n" );
        if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_sonar,0)){

                if ( !get_sensor_value0(sn_sonar, &value )) {

                        value = init_sonar + 90;

                }

                sonar_val = value-init_sonar;
                sonar_abs = abs(sonar_val);

        }

	while (sonar_abs > 2.0) {

		//motors stop
		set_tacho_command_inx( sensor_left, TACHO_STOP );

		set_tacho_command_inx( sensor_right, TACHO_STOP );
		if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_sonar,0)){

			if ( !get_sensor_value0(sn_sonar, &value )) {

				value = init_sonar + 90;

			}

			sonar_val = value-init_sonar;
			sonar_abs = abs(sonar_val);

			//printf( "\r(%f) \n", sonar_val);
			//fflush( stdout );

		}
                        set_tacho_position_sp( sensor_right, sonar_val );
			set_tacho_position_sp( sensor_left, -sonar_val );
			multi_set_tacho_command_inx( motor, TACHO_RUN_TO_REL_POS );
			//Sleep(sonar_abs*5);
			do {
                                get_tacho_state_flags( sensor_right, &state );
                        } while (state);

	}
