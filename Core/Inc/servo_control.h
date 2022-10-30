/*
 * servo_control.h
 *
 *  Created on: Sep 23, 2022
 *      Author: tbrad
 */

#ifndef INC_SERVO_CONTROL_H_
#define INC_SERVO_CONTROL_H_

#include "stm32l4xx_hal.h"
#include <stdint.h>
#include <stdlib.h>
#include "rng_helper.h"
#include "cmsis_os.h"


#define SERVO_PLAYER 1
#define SERVO_CPU	 2

#define POS0 0
#define POS1 1
#define POS2 2
#define POS3 3
#define POS4 4
#define POS5 5

/*
 * SERVO_STATE
 *     servo can either be moving or stable
 */
typedef enum
{
	CALIBRATING,
	MOVING,
	STABLE
} SERVO_STATE;


/*
 * ServoControl typedef
 * 		each servo has a position and state
 */
typedef struct
{
	uint8_t ID;
	uint8_t servo_position;
	SERVO_STATE SERVO_STATE;
	uint16_t duty_cycle_arr[6];
} Servo_t;

void init_servo(Servo_t *servo, uint8_t servo_id);
void set_duty_cycles(Servo_t *servo);
void move_servo_right(Servo_t *servo);
void move_servo_left(Servo_t *servo);
void move_servo_to_pos(Servo_t *servo, uint8_t position);
void decrement_pos0_duty_cycle(Servo_t *servo);
void increment_pos0_duty_cycle(Servo_t *servo);
void decrement_pos5_duty_cycle(Servo_t *servo);
void increment_pos5_duty_cycle(Servo_t *servo);
void move_cpu_to_random_pos();
#endif /* INC_SERVO_CONTROL_H_ */
