/*
 * rng_helper.h
 *
 *  Created on: Oct 22, 2022
 *      Author: tbrad
 */

#ifndef INC_RNG_HELPER_H_
#define INC_RNG_HELPER_H_

// include header files
#include "stm32l4xx_hal.h"

// global variables
extern RNG_HandleTypeDef hrng;

// constant defines
#define NUM_POSITIONS 	6
#define ONE_SEC			1000
#define FOUR_SEC		4000

// functions
uint8_t get_random_position();
uint16_t get_random_delay();

#endif /* INC_RNG_HELPER_H_ */
