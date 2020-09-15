/*
 * globals.h
 *
 *  Created on: Dec 26, 2015
 *      Author: dchiu
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

int num_threads;	// number of customer threads
int now_serving;	// customer's ID who is being served

//TODO - declare some semaphores
sem_t* customer_entering; //bartener waits for customer to enter
sem_t* place_order; //bartender waits for customer to order
sem_t* drink_done; //wait for drink if done staring at art
sem_t* drink_payment; //bartender waits for drink payment
sem_t* money_received; //customer wait until Bartender confirms payment
sem_t* customer_order; //bartener waits for next order
sem_t* mutex; //mutual exclusion in bar

#endif /* GLOBALS_H_ */
