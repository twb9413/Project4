/*
 * Game_Control.h
 *
 *  Created on: Oct 22, 2022
 *      Author: tbrad
 */

#ifndef INC_GAME_CONTROL_H_
#define INC_GAME_CONTROL_H_

#include <stdint.h>
#include <stm32l4xx_hal.h>

#include "servo_control.h"
#include "MFS.h"
#include "rng_helper.h"

// buttons which control game
#define ShieldBtn1_Pin GPIO_PIN_1
#define ShieldBtn2_Pin GPIO_PIN_4
#define ShieldBtn3_Pin GPIO_PIN_0

// game states for easier game control
typedef enum
{
	SETUP, // initial setup
	CAL_P0, // calibrate player position 0
	CAL_P5, // calibrate player position 5
	CAL_CP0, // calibraet cpu pos 0
	CAL_CP5, // calibrate cpu pos 5
	PLAYING, // play the game
	OVER // five rounds have happened
} GAME_STATE;

// game type definition
typedef struct
{
	GAME_STATE game_state;
	uint16_t score;
	uint8_t round;
} Game_t;

void game_task();
void display_task();
void score_task();

#endif /* INC_GAME_CONTROL_H_ */
