/*
 * teller.h
 *
 *  Created on: Oct 15, 2020
 *      Author: nkell
 */

#ifndef INC_TELLER_H_
#define INC_TELLER_H_

void teller_task(void *params);

typedef struct{
	uint32_t wait_start_time;
	uint32_t wait_end_time;
} teller_metrics_t;

void end_teller();

#endif /* INC_TELLER_H_ */
