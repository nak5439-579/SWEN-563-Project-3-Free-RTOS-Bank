/*
 * helper.c
 *
 *  Created on: Oct 19, 2020
 *      Author: nkell
 */

#include "FreeRTOS.h"
#include "task.h"
#include "rng.h"
#include "helper.h"
#include "manager.h"
#include "tim.h"
#include "usart.h"
#include "string.h"
#include "gpio.h"
#include "semaphores.h"

void wait_for_RNG_ready(){
	while(!(RNG->SR & RNG_SR_DRDY_Msk)){
		vTaskDelay(1);//if it isn't ready, wait one tick and check again
	}
}

int get_simulation_time(uint32_t end){
	uint32_t start, delta, mins, hours, final;
	start = get_start_time();
	delta = end - start;
	mins = (delta / 100000);
	hours = mins / 60;
	mins = mins % 60;
	hours = hours + 9;
	final = (hours * 100) + mins;
	return final;
}

int ms_to_min(uint32_t count){
	uint32_t millis;
	int mins, secs, final;
	millis = count / 1000;
	secs = millis / 1.66667;
	mins = secs / 60;
	secs = secs % 60;
	final = (mins * 100) + secs;
	return final;
}

void print_serial(char *str){
	xSemaphoreTake(xUSART, portMAX_DELAY);
	HAL_UART_Transmit(&huart2, str, strlen(str), portMAX_DELAY);
	xSemaphoreGive(xUSART);
}

