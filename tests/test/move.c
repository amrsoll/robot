#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "move.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "ev3_port.h"


#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//float init_sonar;
int init_position;

void move(uint8_t sn_sonar,
	uint8_t sensor_left,
	uint8_t sensor_right,
	uint8_t motor[2],
	float init_sonar,
	float distance)
{
	//FLAGS_T state;
	float sonar_abs,
	float real_sonar_val;
	float measured_sonar_val;
	int maximum_robot_speed;
	int position;
	int save_curr_position;
	//uint8_t sensor_left = motors[0];
	//uint8_t sensor_right = motors[1];

	get_tacho_position(sensor_right, &position);
	distance = (int) distance * 360/200; //TRy to change it

	int final_position = position + distance;
	multi_set_tacho_command_inx(motor,TACHO_RUN_FOREVER);
	while(position<final_position){

		if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_sonar,0)){

			if ( !get_sensor_value0(sn_sonar, &measured_sonar_val )) {
				measured_sonar_val = init_sonar;
			}

			real_sonar_val = measured_sonar_val-init_sonar;
			sonar_abs = abs(real_sonar_val);

			 printf( "\r(%f) \n", real_sonar_val);
			//  fflush( stdout );
		} else {
			printf("failed to find the sonar sensor");
			exit(EXIT_FAILURE);
		}
		//Stop if an obstacle is too close
		if (sonar_abs > 2.0) {
			get_tacho_position(sensor_right, &save_curr_position);
			while (sonar_abs > 2.0) {

				//motors stop on both side
				set_tacho_command_inx( sensor_left, TACHO_STOP );
				set_tacho_command_inx( sensor_right, TACHO_STOP );
				if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_sonar,0)){

					if ( !get_sensor_value0(sn_sonar, &measured_sonar_val )) {

						measured_sonar_val = init_sonar;

					}

					real_sonar_val = measured_sonar_val-init_sonar;
					sonar_abs = abs(real_sonar_val);

					printf( "\r(%f) I will stop\n", real_sonar_val);
					//fflush( stdout );

				}
				get_tacho_max_speed(sensor_right,&maximum_robot_speed);
				printf("Robot max speed iis = %d\n",&maximum_robot_speed);

				set_tacho_speed_sp( sensor_right, maximum_robot_speed / 10 );
				set_tacho_speed_sp( sensor_left, maximum_robot_speed / 10 );
				set_tacho_position_sp( sensor_right, real_sonar_val-2 );
				set_tacho_position_sp( sensor_left, -real_sonar_val+2 );
				multi_set_tacho_command_inx( motor, TACHO_RUN_TO_REL_POS );
				//Sleep(gyro_abs*5);
				//do {
					//get_tacho_state_flags( sensor_right, &state );
				//} //while (state);
			}
			get_tacho_position(sensor_right, &position);
			save_curr_position = position - save_curr_position;
			final_position = final_position + save_curr_position;
			set_tacho_speed_sp( sensor_right, maximum_robot_speed / 3 );
			set_tacho_speed_sp( sensor_left, maximum_robot_speed / 3);
			multi_set_tacho_command_inx(motor,TACHO_RUN_FOREVER);
		}


		get_tacho_position(sensor_right, &position);

	}

	set_tacho_command_inx( sensor_left, TACHO_STOP );
	set_tacho_command_inx( sensor_right, TACHO_STOP );

}
