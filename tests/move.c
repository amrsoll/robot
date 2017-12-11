#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
//#include "move.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "ev3_port.h"


#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

float init_sonar;
int init_position;

void move(uint8_t sn_sonar, uint8_t motors[2], float distance)

{
	FLAGS_T state;
	float sonar_abs;
	float sonar_val;
	float value;
	int maximum_robot_speed;
	int position;
	int save_curr_position;
	uint8_t motor_left = motors[0];
	uint8_t motor_right = motors[1];

	get_tacho_position(motor_right, &position);
	distance = (int) distance * 360/200; //TRy to change it

	int final_position = position + distance;
	multi_set_tacho_command_inx(motors,TACHO_RUN_FOREVER);
	while(position<final_position){

		if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_sonar,0)){

			if ( !get_sensor_value0(sn_sonar, &value )) {
				value = init_sonar;
			}

			sonar_val = value-init_sonar;
			sonar_abs = abs(sonar_val);

			//  printf( "\r(%f) \n", sonar_val);
			//  fflush( stdout );

		}
		//Stop if an obstacle is too close
		if (sonar_abs > 2.0) {
			get_tacho_position(motor_right, &save_curr_position);
			while (sonar_abs > 2.0) {

				//motors stop on both side
				set_tacho_command_inx( motor_left, TACHO_STOP );
				set_tacho_command_inx( motor_right, TACHO_STOP );
				if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_sonar,0)){

					if ( !get_sensor_value0(sn_sonar, &value )) {

						value = init_sonar;

					}

					sonar_val = value-init_sonar;
					sonar_abs = abs(sonar_val);

					//printf( "\r(%f) \n", sonar_val);
					//fflush( stdout );

				}
				get_tacho_max_speed(motor_right,&maximum_robot_speed);
				printf("Robot max speed is = %d\n",&maximum_robot_speed);

				set_tacho_speed_sp( motor_right, maximum_robot_speed / 10 );
				set_tacho_speed_sp( motor_left, maximum_robot_speed / 10 );
				set_tacho_position_sp( motor_right, sonar_val-2 );
				set_tacho_position_sp( motor_left, -sonar_val+2 );
				multi_set_tacho_command_inx( motors, TACHO_RUN_TO_REL_POS );
				//Sleep(gyro_abs*5);
				do {
					get_tacho_state_flags( motor_right, &state );
				} while (state);
			}
			get_tacho_position(motor_right, &position);
			save_curr_position = position - save_curr_position;
			final_position = final_position + save_curr_position;
			set_tacho_speed_sp( motor_right, maximum_robot_speed / 3 );
			set_tacho_speed_sp( motor_left, maximum_robot_speed / 3);
			multi_set_tacho_command_inx(motors,TACHO_RUN_FOREVER);
		}


		get_tacho_position(motor_right, &position);

	}

	set_tacho_command_inx( motor_left, TACHO_STOP );
	set_tacho_command_inx( motor_right, TACHO_STOP );

}
