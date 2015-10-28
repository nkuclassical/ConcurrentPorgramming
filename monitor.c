#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "monitor.h"
#include "cart.h"
#include "q.h"




void monitor_init(){
	readyCart=NULL;
	crossingCart=NULL;
}

void monitor_arrive(struct cart_t* cart){
	pthread_mutex_lock(&mutex);
	if(readyCart!=NULL){
		printf("Cart %d %c must wait util his turn\n", cart->num,cart->dir);
		while(readyCart!=NULL){
			pthread_cond_wait(&ready,&mutex);		
		}
	}
	printf("Cart %d %c become next ready cart\n", cart->num, cart->dir);
	readyCart=cart;

	pthread_mutex_unlock(&mutex);
}

void monitor_cross(struct cart_t* cart){
	int i;
	pthread_mutex_lock(&mutex);
	if(crossingCart!=NULL){
		printf("Cart %d %c must wait before entering intersection\n", cart->num,cart->dir);
		while(crossingCart!=NULL){
			pthread_cond_wait(&crossing,&mutex);
		}
	}
	readyCart=NULL;
	crossingCart=cart;
	pthread_cond_signal(&ready);
	pthread_mutex_unlock(&mutex);
	printf("Cart %d %c enters intersection\n", cart->num, cart->dir);	
	for(i=0;i<1;i++){
		printf("Cart %d %c is crossing.\n", cart->num, cart->dir);
		sleep(1);
	}
	printf("Cart %d %c has crossed intersection\n", cart->num, cart->dir);
}

void monitor_leave(struct cart_t*cart){
	pthread_mutex_lock(&mutex);
	crossingCart=NULL;
	printf("Cart %d %c leaves intersection\n", cart->num, cart->dir);
	pthread_cond_signal(&crossing);
	pthread_mutex_unlock(&mutex);
}

void monitor_shutdown(){
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&ready);
	pthread_cond_destroy(&crossing);
}