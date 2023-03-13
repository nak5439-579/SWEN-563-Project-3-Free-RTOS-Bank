/*
 * semaphores.h
 *
 *  Created on: Oct 15, 2020
 *      Author: nkell
 */

#ifndef INC_SEMAPHORES_H_
#define INC_SEMAPHORES_H_
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "customer.h"


extern SemaphoreHandle_t xUSART;
extern SemaphoreHandle_t xQueue;
extern SemaphoreHandle_t xStats;
extern SemaphoreHandle_t xRNG;
extern SemaphoreHandle_t xBuffer;
extern QueueHandle_t customer_queue;
extern TaskHandle_t customer_task_handle;
extern TaskHandle_t teller_1_task_handle;
extern TaskHandle_t teller_2_task_handle;
extern TaskHandle_t teller_3_task_handle;
extern TaskHandle_t manager_task_handle;
extern char end_flag;
extern char customer_finished;
extern char teller_1_finished;
extern char teller_2_finished;
extern char teller_3_finished;

#endif /* INC_SEMAPHORES_H_ */
