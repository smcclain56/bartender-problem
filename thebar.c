/*
 * TheBar.c
 *
 *  Created on: Dec 26, 2015
 *      Author: dchiu
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include "globals.h"
#include "customer.h"
#include "bartender.h"


void printBanner();
void init();
void cleanup();


/**
 * Main function
 */
int main(int argc, char** argv)
{
	//Error message if num_threads is not entered
	int i;
	if(argv[1] == NULL){
		printf("Number of threads must be entered\n");
		return 0;
	}

	//Initialize num_threads and threads for bartender and customers
	num_threads = atoi(argv[1]);
	pthread_t btid;
	pthread_t tid[num_threads];
	int numCus[num_threads];

	//Fill in numCus array
	for(i=0;i<num_threads;i++){
		numCus[i] = i;
	}

	//Print banner and initialize semaphroes
	printBanner();
	init();

	//fire off bartender thread
	pthread_create(&btid,NULL,bartender,NULL);

	//fire off customer threads
	for(i=0;i<num_threads;i++){
		pthread_create(&tid[i],NULL,customer,(void *)&numCus[i]);
		sleep(1);
	}

	//reap customer threads
	for(i=0;i<num_threads;i++){
		pthread_join(tid[i],NULL);
		sleep(1);
	}

	//reap bartender thread
	pthread_join(btid,NULL);

	//cleanup and destroy semaphores
	cleanup();
	return 0;
}


/**
 * Prints the activity banner.
 * Do not touch.
 */
void printBanner() {
	printf("Customer:\t\t\t\t\t\t\t\t\t\t| Employee:\n");
	printf("Traveling\tArrived\t\tOrdering\tBrowsing\tAt Register\tLeaving");
	printf("\t| Waiting\tMixing Drinks\tAt Register\tPayment Recv\n");
	printf("----------------------------------------------------------------------------------------+");
	printf("------------------------------------------------------------\n");
}


/**
 * Create and initialize semaphores
 */
void init()
{
	//TODO - unlink semaphores
	sem_unlink("/customer_entering");
	sem_unlink("/drink_done");
	sem_unlink("/drink_payment");
	sem_unlink("/money_received");
	sem_unlink("/customer_order");
	sem_unlink("/mutex");
	sem_unlink("/place_order");

	//TODO - create semaphores
	customer_entering = sem_open("/customer_entering", O_CREAT, 0600, 0);
	drink_done = sem_open("/drink_done", O_CREAT, 0600, 0);
	drink_payment = sem_open("/drink_payment", O_CREAT, 0600, 0);
	money_received = sem_open("/money_received", O_CREAT, 0600, 0);
	customer_order = sem_open("/customer_order", O_CREAT, 0600, 0);
	mutex = sem_open("/mutex", O_CREAT, 0600, 1); //allows only 1 in for mutual exclusion
	place_order = sem_open("/place_order", O_CREAT, 0600, 0);
}


/**
 * Cleanup and destroy semaphores
 */
void cleanup()
{
	//TODO - close semaphores
	sem_close(customer_entering);
	sem_close(drink_done);
	sem_close(drink_payment);
	sem_close(money_received);
	sem_close(customer_order);
	sem_close(mutex);
	sem_close(place_order);

	//unlink semaphores
	sem_unlink("/customer_entering");
	sem_unlink("/drink_done");
	sem_unlink("/drink_payment");
	sem_unlink("/money_received");
	sem_unlink("/customer_order");
	sem_unlink("/place_order");
	sem_unlink("/mutex");
}
