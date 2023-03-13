/*
 * customer.h
 *
 *  Created on: Oct 15, 2020
 *      Author: nkell
 */

#ifndef INC_CUSTOMER_H_
#define INC_CUSTOMER_H_

typedef struct{
	uint32_t time_of_arival;
	uint32_t time_enter_queue;
	uint32_t time_of_service;
	uint16_t time_for_transaction;
} customer_t;

void customer_task(void *params);


#endif /* INC_CUSTOMER_H_ */
