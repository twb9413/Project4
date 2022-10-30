/*
 * MFS.c
 *
 *  Created on: Oct 26, 2022
 *      Author: tbrad
 */

/*
 * MFS_functions.c
 * this file is used hold the sheild operations
 *  Created on: Oct 16, 2022
 *      Author: tbrad
 */


#include "MFS.h"


/* Segment byte maps for numbers 0 to 9 */
static const char SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};
/* Byte maps to select digit 1 to 4 */
static const char SEGMENT_SELECT[] = {0xF1,0xF2,0xF4,0xF8};

/*
 * shiftOut
 *     credit to MFS_EXAMPLE Author
 */
void shiftOut(uint8_t val)
{
	for(int ii=0x80; ii; ii>>=1)
	{
	  HAL_GPIO_WritePin(SHLD_D7_SEG7_Clock_GPIO_Port,SHLD_D7_SEG7_Clock_Pin, GPIO_PIN_RESET);    // clear clock pin
	  if(ii & val)						                                                     // if this bit in `value` is set
		  HAL_GPIO_WritePin(SHLD_D8_SEG7_Data_GPIO_Port, SHLD_D8_SEG7_Data_Pin,GPIO_PIN_SET);  //   set it in shift register
	  else
		  HAL_GPIO_WritePin(SHLD_D8_SEG7_Data_GPIO_Port, SHLD_D8_SEG7_Data_Pin,GPIO_PIN_RESET); 	//   else clear it
	  HAL_GPIO_WritePin(SHLD_D7_SEG7_Clock_GPIO_Port,SHLD_D7_SEG7_Clock_Pin, GPIO_PIN_SET);       // set clock pin
	}
}

/*
 * WriteNUmberToSegment
 *     Write a decimal number between 0 and 9 to one of the 4 digits of the display
 *     credit to MFS_EXAMPLE Author
 */
void WriteNumberToSegment(int Segment, int Value)
{
	HAL_GPIO_WritePin(SHLD_D4_SEG7_Latch_GPIO_Port, SHLD_D4_SEG7_Latch_Pin, GPIO_PIN_RESET);
	shiftOut(SEGMENT_MAP[Value]);
	shiftOut(SEGMENT_SELECT[Segment] );
	HAL_GPIO_WritePin(SHLD_D4_SEG7_Latch_GPIO_Port, SHLD_D4_SEG7_Latch_Pin, GPIO_PIN_SET);
}


void display_score(uint16_t score)
{
	int onesNum = 		(score / 1) % 10;
	int tensNum = 		(score / 10) % 10;
	int hundredsNum = 	(score / 100) % 10;
	int thousandsNum = 	(score / 1000) % 10;
	WriteNumberToSegment(THOUSANDS, thousandsNum);
	WriteNumberToSegment(HUNDREDS, hundredsNum);
	WriteNumberToSegment(TENS, tensNum);
	WriteNumberToSegment(ONES, onesNum);
}



