/*
 * customer.c
 *
 *  Created on: Oct 16, 2020
 *      Author: nkell
 */
#include "FreeRTOS.h"
#include "rng.h"
#include "semphr.h"
#include "semaphores.h"
#include "helper.h"
#include "usart.h"
#include "manager.h"
#include "string.h"
#include "task.h"
#include "cmsis_os.h"


//this task will handle creating customers and adding them to the queue
void customer_task(void *params){



	uint16_t time_until_next_arival;
	uint16_t time_for_transaction;
	uint32_t random_num;
	uint32_t lower_16_mask = 0x0000FFFF;
	customer_t new_cust;

	while(1){

		//wait until the RNG data is ready


		//print_serial(w);
		//vTaskDelay(1000);



		//wait_for_RNG_ready();
		//random_num = RNG->DR;
		random_num = HAL_RNG_GetRandomNumber(&hrng);

		//get the random value in range
		//range is 100ms - 400ms (1min - 4min)
		//# % 300 gives 0 - 299, so use 301 for range 0 - 300,
		//then add 100 to bring the floor up to 100, giving results the range 100 - 400

		//range is 50ms - 800ms (0.5min - 8min)

		//this next part is kind of awkward, but since we don't *NEED* the entire
		//32bits of entropy, so we can save a repeated call to the register
		//(and the required wait for the data to be ready) by treating the upper and lower
		//16 bits as separate numbers. after all, it's all random, isn't it?
		time_until_next_arival = ((random_num & lower_16_mask) % 301) + 100;
		time_for_transaction = ((random_num >> 16) % 751) + 50;

		new_cust.time_for_transaction = time_for_transaction;
		//wait the appropriate amount of time before "arriving" at the bank
		new_arival(new_cust);
		vTaskDelay(pdMS_TO_TICKS(time_until_next_arival));

		if(!end_flag){
			customer_finished = 0;
			while(1){
				vTaskDelay(50);
			}
		}
	}
}
