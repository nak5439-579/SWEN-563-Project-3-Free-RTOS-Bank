/*
 * manager.h
 *
 *  Created on: Oct 15, 2020
 *      Author: nkell
 */

#ifndef INC_MANAGER_H_
#define INC_MANAGER_H_
#include "customer.h"

//you may ask yourself: why so much data here?
//it makes reporting it easier, and it makes sense that the
//tellers don't have to report their own metrics,
//leave that up to the managers or whoever's watching them
typedef struct{
	int num_customers;
	int num_customers_received;
	int num_customers_teller_1;
	int num_customers_teller_2;
	int num_customers_teller_3;
	char status_teller_1;
	char status_teller_2;
	char status_teller_3;
	uint32_t average_customer_time_in_queue;
	uint32_t average_transaction_time;
	uint32_t average_teller_wait_for_customer_time;
	uint32_t maximum_customer_wait_in_queue_time;
	uint32_t maximum_teller_wait_for_customer_time;
	uint32_t maximum_transaction_time;
	uint32_t max_queue_length;
} bank_metrics_t;


void manager_task(void *params);
void set_start_and_close_time();
uint32_t get_start_time();
uint32_t get_close_time();
void new_arival(customer_t new_arival);
void print_arival_information(customer_t cust);
void print_customer_approaches_teller();
void print_queue_info();
void print_teller_info();
void make_teller_busy();
void free_teller();
void end_simulation();
int are_tellers_busy();
void new_cust_received();
void update_average_customer_wait_in_queue(uint32_t start, uint32_t end);
void update_average_transaction_time(uint32_t transaction_time);
void update_average_teller_wait_for_customer(uint32_t start, uint32_t end);
#endif /* INC_MANAGER_H_ */
