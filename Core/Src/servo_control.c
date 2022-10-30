/*
 * servo_control.c
 *
 *	This file includes all primitives for adjusting each servo position
 *
 *  Created on: Sep 23, 2022
 *      Author: tbrad
 */

#include "servo_control.h"

Servo_t servo_player;
Servo_t servo_cpu;

/*
 * initialize this servo's parameters
 */
void init_servo(Servo_t *servo, uint8_t servo_id)
{
	servo->ID = servo_id;
	servo->SERVO_STATE = STABLE;
	servo->servo_position = 0;
	servo->duty_cycle_arr[POS0] = 70; // estimate of position 0 to be calibrated from
	servo->duty_cycle_arr[POS5] = 180; // initial guess for position 5 to be calibrated from
}

/*
 * set_duty_cycles(servo)
 * 		calculate the duty cycles to be used at each position
 */
void set_duty_cycles(Servo_t *servo)
{
	// compute the range of motion after calibration
	uint16_t range = (servo->duty_cycle_arr[POS5] - servo->duty_cycle_arr[POS0]);

	// separate that range into equal steps, int division so will be some error
	uint16_t step = range / 5;

	// set all positional duty cycles
	servo->duty_cycle_arr[POS1] = servo->duty_cycle_arr[POS0] + (POS1 * step);
	servo->duty_cycle_arr[POS2] = servo->duty_cycle_arr[POS0] + (POS2 * step);
	servo->duty_cycle_arr[POS3] = servo->duty_cycle_arr[POS0] + (POS3 * step);
	servo->duty_cycle_arr[POS4] = servo->duty_cycle_arr[POS0] + (POS4 * step);
}

/*
 * set_pwm
 * 	function to inject new duty cycle to pwm timer
 */
void set_pwm(uint8_t servo, uint16_t duty)
{
	switch ( servo )
	{
		case 1:
			TIM3->CCR1 = duty;
			break;
		case 2:
			TIM3->CCR2 = duty;
			break;
		default:
			break;
	}
}

/*
 * move_servo_right
 * adjust the duty cycle of the servo to move it right by one position
 */
void move_servo_right(Servo_t *servo)
{
	// where is the servo rn?
	uint8_t current_position = servo->servo_position;
	switch(current_position)
	{
	case POS0:
		// cannot move further right from this position
		break;
	default:
		servo->SERVO_STATE = MOVING;
		set_pwm(servo->ID, servo->duty_cycle_arr[current_position - 1]);
		servo->servo_position -= 1;
		servo->SERVO_STATE = STABLE;
		break;
	}

}

/*
 * move_servo_left
 * adjust the duty cycel of the servo to move it left by one position
 */
void move_servo_left(Servo_t *servo)
{
	// where is the servo rn?
	uint8_t current_position = servo->servo_position;
	switch(current_position)
	{
	case POS5:
		// cannot move further left from this position
		break;
	default:
		servo->SERVO_STATE = MOVING;
		set_pwm(servo->ID, servo->duty_cycle_arr[current_position + 1]);
		servo->servo_position += 1;
		servo->SERVO_STATE = STABLE;
		break;
	}
}

/*
 * move_servo_to_pos
 *  Move the servo to a specific position
 */
void move_servo_to_pos(Servo_t *servo, uint8_t position)
{
	servo->SERVO_STATE = MOVING;
	servo->servo_position = position;
	set_pwm(servo->ID, servo->duty_cycle_arr[position]);
	servo->SERVO_STATE = STABLE;
}

void move_cpu_to_random_pos()
{
	// get a random position
	uint8_t position = get_random_position();
	// ensure not moving to same position
	while(position == servo_cpu.servo_position)
	{
		position = get_random_position();
	}
	move_servo_to_pos(&servo_cpu, position);
}

/*
 * --------------------------------------------------------------------
 * Increment and Decrement duty cycle functions
 * 		calibration routine helper functions
 */
void decrement_pos0_duty_cycle(Servo_t *servo)
{
	servo->duty_cycle_arr[POS0] -= 2;
	set_pwm(servo->ID, servo->duty_cycle_arr[POS0]);
}
void increment_pos0_duty_cycle(Servo_t *servo)
{
	servo->duty_cycle_arr[POS0] += 2;
	set_pwm(servo->ID, servo->duty_cycle_arr[POS0]);
}
void decrement_pos5_duty_cycle(Servo_t *servo)
{
	servo->duty_cycle_arr[POS5] -= 2;
	set_pwm(servo->ID, servo->duty_cycle_arr[POS5]);
}
void increment_pos5_duty_cycle(Servo_t *servo)
{
	servo->duty_cycle_arr[POS5] += 2;
	set_pwm(servo->ID, servo->duty_cycle_arr[POS5]);
}
//---------------------------------------------------------------------
