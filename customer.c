/*
 * customer.c
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
#include "customer.h"

/**
 * This is what the thread will call.
 * Do not touch.
 */
void* customer(void* args)
{
	unsigned int *custID = (unsigned int*) args;
	custTravelToBar(*custID);
	custArriveAtBar(*custID);
	custPlaceOrder();
	custBrowseArt();
	custAtRegister();
	custLeaveBar();
	return NULL;
}


/**
 * Each customer takes a random amount of time between 20 ms and 5000 ms to travel to the bar.
 */
void custTravelToBar(unsigned int custID)
{
	//Customer takes a random amount of time between 20 ms and 5000 ms to travel to bar
	int upper = 5000;
	int lower = 20;
	int random = (rand() % (upper - lower + 1)) + lower;
	usleep(random*1000);

	// Prints Id of customer traveling
	printf("Cust %u\t\t\t\t\t\t\t\t\t\t\t|\n", custID);
}


/**
 * If there is already another customer in the bar the current customer has
 * to wait until bar is empty before entering.
 */
void custArriveAtBar(unsigned int custID)
{
	//Mutex only allows one customer in the bar
	sem_wait(mutex);

	//updates which customer entered the bar
	now_serving = custID;

	printf("\t\tCust %u\t\t\t\t\t\t\t\t\t|\n", custID);

	// Alerts bartender to stop waiting for a customer to arrive
	sem_post(customer_entering);
}


/**
 * The customer in the bar places an order
 */
void custPlaceOrder()
{
	printf("\t\t\t\tCust %u\t\t\t\t\t\t\t|\n", now_serving);

	//Alerts bartender that the order as been placed
	sem_post(place_order);
}


/**
 * The customer in the bar can browse the wall art for a random amount of time between 3ms and 4000ms.
 */
void custBrowseArt()
{
	//Customer browse wall art for a random amount of time between 3 ms and 4000 ms
	int upper = 4000;
	int lower = 3;
	int random = (rand() % (upper - lower + 1)) + lower;
	usleep(random*1000);

	//Then customer is ready for drink
	printf("\t\t\t\t\t\tCust %u\t\t\t\t\t|\n", now_serving);
}


/**
 * If their drink is not ready by the time they are done admiring the art they must wait
 * until the bartender has finished. When the bartender is finished, the customer pays.
 *
 */
void custAtRegister()
{
	//Customer waits on drink to be done if the bartender hasn't finished making it
	sem_wait(drink_done);

	//prints customer being served
	printf("\t\t\t\t\t\t\t\tCust %u\t\t\t|\n", now_serving);

	//alerts bartender that payment was made
	sem_post(drink_payment);

	//waits on bartender to check if correct amount of money was given
	sem_wait(money_received);
}


/**
 * The customer in the bar leaves the bar.
 */
void custLeaveBar()
{
	//Prints customer leaving
	printf("\t\t\t\t\t\t\t\t\t\tCust %u\t|\n", now_serving);

	//Updates mutex so next customer may now enter the bar
	sem_post(mutex);
}
