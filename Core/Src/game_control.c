/*
 * game_control.c
 *
 *	This file is responsible for running the game
 *  Created on: Oct 22, 2022
 *      Author: tbrad
 */

#include "game_control.h"

// file variables
Game_t game;
extern Servo_t servo_player;
extern Servo_t servo_cpu;
extern UART_HandleTypeDef huart2;

// function prototypes
void handle_btn1();
void handle_btn2();
void handle_btn3();

/*
 * start_game()
 * 		game initializer
 */
void start_game()
{
	HAL_UART_Transmit(&huart2, (uint8_t *) "\n\rPress button 2 to calibrate Player Servo", 42, HAL_MAX_DELAY);
	game.game_state = SETUP; // player servo configuration will happen first
	game.score = 0;
	game.round = 0;
}

/*
 * get_game_state()
 * 		return the state of the game
 * 		i.e: SETUP, PLAYING
 */
GAME_STATE get_game_state()
{
	return game.game_state;
}

/*
 * get_game_score()
 * 		return the current score
 */
uint32_t get_game_score()
{
	return game.score;
}

/**
 * set the game state
 */
void set_game_state(GAME_STATE game_state)
{
	game.game_state = game_state;
}


/*
 * Callback function for GPIO interrupts
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
	case ShieldBtn1_Pin:
		HAL_UART_Transmit(&huart2, (uint8_t *) "\n\rbtn1", 6, HAL_MAX_DELAY);
		handle_btn1();
		break;
	case ShieldBtn2_Pin:
		HAL_UART_Transmit(&huart2, (uint8_t *) "\n\rbtn2", 6, HAL_MAX_DELAY);
		handle_btn2();
		break;
	case ShieldBtn3_Pin:
		HAL_UART_Transmit(&huart2, (uint8_t *) "\n\rbtn3", 6, HAL_MAX_DELAY);
		handle_btn3();
		break;
	}
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin); // having issue with shield reading 2 button presses instead of 1, clear the interrupt here
}

/*
 * handle_btn1()
 * 	button 1 pressed, determine its functionality depending on game state
 */
void handle_btn1()
{
	switch(game.game_state)
	{
	case SETUP:
		// nothing to do in this state
		break;
	case CAL_P0:
		increment_pos0_duty_cycle(&servo_player);
		break;
	case CAL_P5:
		increment_pos5_duty_cycle(&servo_player);
		break;
	case CAL_CP0:
		increment_pos0_duty_cycle(&servo_cpu);
		break;
	case CAL_CP5:
		increment_pos5_duty_cycle(&servo_cpu);
		break;
	case PLAYING:
		// btn1 to move right
		HAL_UART_Transmit(&huart2, (uint8_t *) "\n\rMOVING LEFT", 13, HAL_MAX_DELAY);
		move_servo_left(&servo_player);
		break;
	case OVER:
		// button should start a new game
		break;
	default:
		break;
	}
}

/*
 * handle_btn2()
 * 	button 2 pressed, determine what it should do depending on the game state
 */
void handle_btn2()
{
	switch(game.game_state)
	{
	case SETUP:
		HAL_UART_Transmit(&huart2, (uint8_t *) "\n\rCALIBRATE PLAYER SERVO POSITION 0", 35, HAL_MAX_DELAY);
		init_servo(&servo_player, SERVO_PLAYER);
		init_servo(&servo_cpu, SERVO_CPU);
		move_servo_to_pos(&servo_player, POS0);
		move_servo_to_pos(&servo_cpu, POS0);
		set_game_state(CAL_P0);
		break;
	case CAL_P0:
		HAL_UART_Transmit(&huart2, (uint8_t *) "\n\rCALIBRATE PLAYER SERVO POSITION 5", 35, HAL_MAX_DELAY);
		move_servo_to_pos(&servo_player, POS5);
		set_game_state(CAL_P5);
		break;
	case CAL_P5:
		HAL_UART_Transmit(&huart2, (uint8_t *) "\n\rCALIBRATE PLAYER CPU POSITION 0", 33, HAL_MAX_DELAY);
		set_duty_cycles(&servo_player);
		set_game_state(CAL_CP0);
		break;
	case CAL_CP0:
		HAL_UART_Transmit(&huart2, (uint8_t *) "\n\rCALIBRATE PLAYER CPU POSITION 5", 33, HAL_MAX_DELAY);
		move_servo_to_pos(&servo_cpu, POS5);
		set_game_state(CAL_CP5);
		break;
	case CAL_CP5:
		HAL_UART_Transmit(&huart2, (uint8_t *) "\n\rCALIBRATED PRESS BTN2 TO START GAME", 37, HAL_MAX_DELAY);
		set_duty_cycles(&servo_cpu);
		move_servo_to_pos(&servo_player, POS0);
		move_servo_to_pos(&servo_cpu, POS0);
		set_game_state(PLAYING);
		break;
	case OVER:
		HAL_UART_Transmit(&huart2, (uint8_t *) "\n\rSTARTING NEW GAME", 19, HAL_MAX_DELAY);
		game.score = 0;
		game.round = 0;
		move_servo_to_pos(&servo_player, POS0);
		move_servo_to_pos(&servo_cpu, POS0);
		set_game_state(PLAYING);
		break;
	default:
		break;
	}
}

/*
 * handle_btn3()
 * 	determine what button 3 should do depending on the game state
 */
void handle_btn3()
{
	switch(game.game_state)
	{
	case SETUP:
		// nothing to do here
		break;
	case CAL_P0:
		decrement_pos0_duty_cycle(&servo_player);
		break;
	case CAL_P5:
		decrement_pos5_duty_cycle(&servo_player);
		break;
	case CAL_CP0:
		decrement_pos0_duty_cycle(&servo_cpu);
		break;
	case CAL_CP5:
		decrement_pos5_duty_cycle(&servo_cpu);
		break;
	case PLAYING:
		HAL_UART_Transmit(&huart2, (uint8_t *) "\n\rMOVING RIGHT", 14, HAL_MAX_DELAY);
		move_servo_right(&servo_player);
		break;
	default:
		break;
	}
}

/**
 * game_task()
 * 	the main game task, play through the rounds and such
 */
void game_task()
{
	start_game();
	uint16_t delay;
	while(1)
	{
		switch(game.game_state)
		{
		case PLAYING:
			delay = get_random_delay();
			vTaskDelay(delay / portTICK_PERIOD_MS);
			if(game.round < 5)
			{
				move_cpu_to_random_pos();
			}
			else
			{
				game.game_state = OVER;
			}
			game.round += 1;
			break;
		default:
			break;
		}
	}
}

/*
 * score_task()
 * 	calculate the score of this game
 */
void score_task()
{
	while(1)
	{
		switch(game.game_state)
		{
		case PLAYING:
			while(game.game_state == PLAYING)
			{
				// increment the score if the player and cpu-servo are not in the same position and the cpu-servo is stable (not moving)
				if(servo_player.servo_position != servo_cpu.servo_position &&
						servo_cpu.SERVO_STATE == STABLE)
					game.score += 1;
				vTaskDelay(10 / portTICK_PERIOD_MS);
			}
			break;
		default:
			break;
		}

	}
}

/*
 * display_task()
 * 	send the score to the seven-segment display
 */
void display_task()
{
	while(1)
	{
		display_score(game.score);
	}
}


