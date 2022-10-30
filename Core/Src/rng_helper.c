/*
 * rng_helper.c
 *
 *  Created on: Oct 22, 2022
 *      Author: tbrad
 */


#include "rng_helper.h"

/*
 * get_random_position()
 * 		return a position between 0-5
 */
uint8_t get_random_position()
{
	uint8_t position;
	uint32_t myrng = HAL_RNG_GetRandomNumber(&hrng);
	position = myrng % NUM_POSITIONS;
	return position;
}

/*
 * get_random_delay()
 * 		return a random delay for the game to use between 1-4 seconds
 */
uint16_t get_random_delay()
{
	uint32_t delay;
	uint32_t myrng = HAL_RNG_GetRandomNumber(&hrng);
	uint32_t range = FOUR_SEC - ONE_SEC;
	delay = ( myrng % range ) + ONE_SEC;
	return delay;
}
