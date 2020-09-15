/*
 * bartender.c
 *
 *  Created on: Dec 26, 2015
 *      Author: dchiu
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include "globals.h"
#include "bartender.h"

/**
 * Code for bartender thread.
 * Do not touch.
 */
void* bartender(void* args)
{
	int i;
	for (i = 0; i<num_threads; i++)
	{
		waitForCustomer();
		makeDrink();
		receivePayment();
	}
	return NULL;
}

/**
 * Waits in a closet until a customer enters.
 */
void waitForCustomer()
{
	// bartender waits for customer to enter
	sem_wait(customer_entering);

	printf("\t\t\t\t\t\t\t\t\t\t\t| Bartender\n");
}


/**
 * When a customer places an order it takes the Bartender
 * a random amount of time between 5 ms and 1000 ms to make the drink.
 */
void makeDrink()
{
	//Bartender waits for customer to place order
	sem_wait(place_order);

	//Bartender takes a random amount of time between 5 ms and 1000 ms to make drink
	int upper = 1000;
	int lower = 5;
	int random = (rand() % (upper - lower + 1)) + lower;
	usleep(random*1000);

	printf("\t\t\t\t\t\t\t\t\t\t\t| \t\tBartender\n");

	// Alerts customer that drink is done
	sem_post(drink_done);
}


/**
 * Gets payment from the correct customer
 */
void receivePayment()
{
	//Bartender waits for customer to pay
	sem_wait(drink_payment);

	//at the register waiting for customer to pay
	printf("\t\t\t\t\t\t\t\t\t\t\t| \t\t\t\tBartender\n");

	//got the payment from the right customer!
	printf("\t\t\t\t\t\t\t\t\t\t\t| \t\t\t\t\t\tBartender\n");
	
	// Bartender alerts customer the correct payment was received
	sem_post(money_received);
}
