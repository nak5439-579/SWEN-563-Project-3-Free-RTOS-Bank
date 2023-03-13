/*
 * manager.c
 *
 *  Created on: Oct 19, 2020
 *      Author: nkell
 */
#include "FreeRTOS.h"
#include "tim.h"
#include "manager.h"
#include "semaphores.h"
#include "usart.h"
#include "helper.h"
#include "gpio.h"
#include "string.h"
#include "task.h"
#include <stdio.h>


bank_metrics_t stats;
uint32_t start_time_count;
uint32_t close_time_count;
char buffer[100];


void manager_task(void *params){

	set_start_and_close_time();

	while(1){
		//print_serial(w);
		if(TIM2->CNT > close_time_count){
			end_simulation();
		}
		vTaskDelay(10);
	}


}

void set_start_and_close_time(){
	start_time_count = TIM2->CNT;
	//7hr * 60min/hr * 100ms/min = 42000 ms * 1000 ticks/ms = 42000000
	close_time_count = start_time_count + 42000000;
}

uint32_t get_start_time(){
	return start_time_count;
}

uint32_t get_close_time(){
	return close_time_count;
}

void new_arival(customer_t new_arival){

	new_arival.time_enter_queue = TIM2->CNT;
	xQueueSendToBack(customer_queue, &new_arival, portMAX_DELAY);
	xSemaphoreTake(xStats, portMAX_DELAY);
	stats.num_customers++;
	if(uxQueueMessagesWaiting(customer_queue) > stats.max_queue_length){
		stats.max_queue_length = uxQueueMessagesWaiting(customer_queue);
	}
	xSemaphoreGive(xStats);
	print_arival_information(new_arival);
	vTaskDelay(2);
	print_queue_info();
	print_teller_info();

}

void print_arival_information(customer_t cust){

	int time = get_simulation_time(cust.time_enter_queue);
	int hours = time / 100;
	int mins = time % 100;

	xSemaphoreTake(xBuffer, portMAX_DELAY);
	sprintf(buffer, "A new customer comes in the bank at %02d:%02d\r\n", hours, mins);
	print_serial(buffer);
	xSemaphoreGive(xBuffer);


}

void print_queue_info(){

	xSemaphoreTake(xBuffer, portMAX_DELAY);
	sprintf((char*)buffer, "There are currently %d people in line\r\n", (int)uxQueueMessagesWaiting(customer_queue));
	print_serial(buffer);
	xSemaphoreGive(xBuffer);
}

void print_teller_info(){
	xSemaphoreTake(xBuffer, portMAX_DELAY);
	sprintf((char *)buffer, "Teller 1 is currently %s\r\n", stats.status_teller_1 ? "busy" : "available");
	print_serial(buffer);
	sprintf((char *)buffer, "Teller 2 is currently %s\r\n", stats.status_teller_2 ? "busy" : "available");
	print_serial(buffer);
	sprintf((char *)buffer, "Teller 3 is currently %s\r\n", stats.status_teller_3 ? "busy" : "available");
	print_serial(buffer);
	xSemaphoreGive(xBuffer);
}

void make_teller_busy(){

	TaskHandle_t running = xTaskGetCurrentTaskHandle();
	if(running == teller_1_task_handle){
		stats.status_teller_1 = 1;
		stats.num_customers_teller_1++;
	}else if(running == teller_2_task_handle){
		stats.status_teller_2 = 1;
		stats.num_customers_teller_2++;
	}else if(running == teller_3_task_handle){
		stats.status_teller_3 = 1;
		stats.num_customers_teller_3++;
	}else{
		xSemaphoreTake(xBuffer, portMAX_DELAY);
		sprintf((char *)buffer, "I done goofed making tellers busy\r\n");
		print_serial(buffer);
		xSemaphoreGive(xBuffer);
	}

}

void free_teller(){

	TaskHandle_t running = xTaskGetCurrentTaskHandle();
	if(running == teller_1_task_handle){
		stats.status_teller_1 = 0;
	}else if(running == teller_2_task_handle){
		stats.status_teller_2 = 0;
	}else if(running == teller_3_task_handle){
		stats.status_teller_3 = 0;
	}else{
		xSemaphoreTake(xBuffer, portMAX_DELAY);
		sprintf((char *)buffer, "I done goofed freeing tellers\r\n");
		print_serial(buffer);
		xSemaphoreGive(xBuffer);
	}

}

void end_simulation(){
	end_flag = 0;
	vTaskDelay(100);
	int time, min, sec;
	sprintf((char *)buffer, "The total number of customers serviced was %d\r\n", stats.num_customers);
	print_serial(buffer);
	sprintf((char *)buffer, "The total number of customers serviced by teller 1 was %d\r\n", stats.num_customers_teller_1);
	print_serial(buffer);
	sprintf((char *)buffer, "The total number of customers serviced by teller 2 was %d\r\n", stats.num_customers_teller_2);
	print_serial(buffer);
	sprintf((char *)buffer, "The total number of customers serviced by teller 3 was %d\r\n", stats.num_customers_teller_3);
	print_serial(buffer);
	time = ms_to_min(stats.average_customer_time_in_queue);
	min = time / 100;
	sec = time % 100;
	sprintf((char *)buffer, "The average time the customer spent waiting in the queue was %02d:%02d\r\n", min, sec);
	print_serial(buffer);
	time = ms_to_min(stats.average_transaction_time);
	min = time / 100;
	sec = time % 100;
	sprintf((char *)buffer, "The average time customers spent with the tellers was %02d:%02d\r\n", min, sec);
	print_serial(buffer);
	time = ms_to_min(stats.average_teller_wait_for_customer_time);
	min = time / 100;
	sec = time % 100;
	sprintf((char *)buffer, "The average time tellers waited for customers was %02d:%02d\r\n", min, sec);
	print_serial(buffer);
	time = ms_to_min(stats.maximum_customer_wait_in_queue_time);
	min = time / 100;
	sec = time % 100;
	sprintf((char *)buffer, "The longest time a customer spent waiting in the queue was %02d:%02d\r\n", min, sec);
	print_serial(buffer);
	time = ms_to_min(stats.maximum_teller_wait_for_customer_time);
	min = time / 100;
	sec = time % 100;
	sprintf((char *)buffer, "The longest time a teller spent waiting for a customer was %02d:%02d\r\n", min, sec);
	print_serial(buffer);
	time = ms_to_min(stats.maximum_transaction_time);
	min = time / 100;
	sec = time % 100;
	sprintf((char *)buffer, "The longest time a customer spent making a transaction was %02d:%02d\r\n", min, sec);
	print_serial(buffer);
	sprintf((char *)buffer, "The longest the queue got was %d\r\n", (int)stats.max_queue_length);
	print_serial(buffer);


	while(1){
		vTaskDelay(10);
	}
}

int are_tellers_busy(){

	if(stats.status_teller_1 || stats.status_teller_2 || stats.status_teller_3){
		return 1;
	}else{
		return 0;
	}

}

void new_cust_received(){
	stats.num_customers_received++;
}

void update_average_customer_wait_in_queue(uint32_t start, uint32_t end){
	uint32_t delta = end - start;//
	if(delta > stats.maximum_customer_wait_in_queue_time){
		stats.maximum_customer_wait_in_queue_time = delta;
	}
	uint32_t old_avg = stats.average_customer_time_in_queue;//placeholder var to make typing easier
	uint32_t new_avg = ((old_avg * (stats.num_customers - 1)) + delta) / stats.num_customers;
	stats.average_customer_time_in_queue = new_avg;
}

void update_average_transaction_time(uint32_t transaction_time){
	if(transaction_time > stats.maximum_transaction_time){
		stats.maximum_transaction_time = transaction_time;
	}
	uint32_t old_avg = stats.average_transaction_time;//placeholder var to make typing easier
	uint32_t new_avg = ((old_avg * (stats.num_customers - 1)) + transaction_time) / stats.num_customers;
	stats.average_transaction_time = new_avg;
}

void update_average_teller_wait_for_customer(uint32_t start, uint32_t end){
	uint32_t delta = end - start;
	if(delta > stats.maximum_teller_wait_for_customer_time){
		stats.maximum_teller_wait_for_customer_time = delta;
	}
	uint32_t old_avg = stats.average_teller_wait_for_customer_time;//placeholder var to make typing easier
	uint32_t new_avg = ((old_avg * (stats.num_customers - 1)) + delta) / stats.num_customers;
	stats.average_teller_wait_for_customer_time = new_avg;
}

void print_customer_approaches_teller(){
	xSemaphoreTake(xBuffer, portMAX_DELAY);
	TaskHandle_t running = xTaskGetCurrentTaskHandle();
	if(running == teller_1_task_handle){
		sprintf((char *)buffer, "The Customer approaches Teller 1\r\n");
	}else if(running == teller_2_task_handle){
		sprintf((char *)buffer, "The Customer approaches Teller 2\r\n");
	}else if(running == teller_3_task_handle){
		sprintf((char *)buffer, "The Customer approaches Teller 3\r\n");
	}else{
		sprintf((char *)buffer, "I done goofed showing what teller was being approached\r\n");
	}
	print_serial(buffer);
	xSemaphoreGive(xBuffer);
}

