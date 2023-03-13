/*
 * helper.h
 *
 *  Created on: Oct 19, 2020
 *      Author: nkell
 */

#ifndef INC_HELPER_H_
#define INC_HELPER_H_

void wait_for_RNG_ready();
int get_simulation_time(uint32_t end);
int ms_to_min(uint32_t count);
void print_serial(char *str);


#endif /* INC_HELPER_H_ */
