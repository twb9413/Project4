/*
 * MFS_functions.h
 *
 *  Created on: Oct 16, 2022
 *      Author: tbrad
 */

#ifndef INC_MFS_FUNCTIONS_H_
#define INC_MFS_FUNCTIONS_H_

#include <stdint.h>
#include "cmsis_os.h"
#include "main.h"

// decimal place values to shield locations
#define ONES 		3
#define TENS 		2
#define HUNDREDS 	1
#define THOUSANDS 	0

void display_score(uint16_t score);
#endif /* INC_MFS_FUNCTIONS_H_ */
