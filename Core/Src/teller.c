/*
 * teller.c
 *
 *  Created on: Oct 19, 2020
 *      Author: nkell
 */
#include "FreeRTOS.h"
#include "teller.h"
#include "manager.h"
#include "customer.h"
#include "queue.h"
#include "tim.h"
#include "usart.h"
#include "task.h"
#include "semaphores.h"
#include "string.h"
#include "helper.h"
#include <stdio.h>

uint8_t buffer2[100];

void teller_task(void *params){

	customer_t servicing;
	teller_metrics_t metrics;
	uint32_t time_of_receival;
	//record time start waiting
	//wait for customer in queue
	//pull cust off queue
	//update metrics
	//delay for time of service
	//"release" the customer
	//loop back
	while(1){
		metrics.wait_start_time = TIM2->CNT;
		xQueueReceive(customer_queue, &servicing, portMAX_DELAY);
		time_of_receival = TIM2->CNT;

		xSemaphoreTake(xStats, portMAX_DELAY);
		make_teller_busy();
		xSemaphoreGive(xStats);
		metrics.wait_end_time = time_of_receival;
		servicing.time_of_service = time_of_receival;
		xSemaphoreTake(xStats, portMAX_DELAY);
		new_cust_received();
		print_customer_approaches_teller();
		update_average_customer_wait_in_queue(servicing.time_enter_queue, servicing.time_of_service);//check max while we're here
		update_average_transaction_time(servicing.time_for_transaction);//update max while we're here
		update_average_teller_wait_for_customer(metrics.wait_start_time, metrics.wait_end_time);//update max while we're here
		xSemaphoreGive(xStats);
		vTaskDelay(pdMS_TO_TICKS(servicing.time_for_transaction));
		xSemaphoreTake(xStats, portMAX_DELAY);
		free_teller();
		xSemaphoreGive(xStats);

		if(!end_flag && !uxQueueMessagesWaiting(customer_queue)){
			end_teller();
			while(1){
				vTaskDelay(50);
			}

		}
	}

}

void end_teller(){

	TaskHandle_t running = xTaskGetCurrentTaskHandle();
	if(running == teller_1_task_handle){
		teller_1_finished = 0;
	}else if(running == teller_2_task_handle){
		teller_2_finished = 0;
	}else if(running == teller_3_task_handle){
		teller_3_finished = 0;
	}else{
		xSemaphoreTake(xBuffer, portMAX_DELAY);
		sprintf((char *)buffer2, "An error occurred finishing tellers busy\r\n");
		print_serial((char *)buffer2);
		xSemaphoreGive(xBuffer);
	}

}

