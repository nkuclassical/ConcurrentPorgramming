#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "cart.h"
#include "monitor.h"
#include "q.h"

void *start_routine(void*);
void usage();
char *process(int, char**);

char dirs[4]={'n','w','s','e'};
char *directions[4]={"North", "West", "South", "East"};
void *start(void *args){
  	
	int index = *(int *)args;
	struct cart_t *cart;
	int time;
	time=0;
	fprintf(stderr,"thread for direction %c starts\n", dirs[index]);
	cart = q_getCart(dirs[index]);

	while (cart!=NULL){
		fprintf(stderr, "thread for direction %c gets cart %i\n", dirs[index], cart->num);
		q_cartHasEntered(dirs[index%4]);
		monitor_arrive(cart);
		monitor_cross(cart);
		monitor_leave(cart);
		time=0;
		/**
			Try to let counterclockwise cart to be the next cart;
			But some carts may got right first, so next cart sometimes
			not follow the counterclockwise order.
		
		index++;
		while(q_cartIsWaiting(dirs[index%4])==0){ 
		 	printf("Now %d\n", index%4);
		 	time++;
		 	if(time==4)break;
		 	index++;
		}
		cart = q_getCart(dirs[index%4]);
		*/
		cart=q_getCart(dirs[index]);

	}
    fprintf(stderr, "thread for direction %c exits\n", dirs[index%4]);
    return NULL;
}

void usage(){
	fprintf(stderr, "./trafficmgr [nesw]+\n");
	exit(EXIT_FAILURE);
}

char* process(int argc,char*argv[]){
	int i;
	char *str;
	if(argc!=2){
		usage();
	}
	str=argv[1];
	for(i = 0; i < strlen(str); i++){
		if(str[i]!='n'&&str[i]!='s'&&str[i]!='e'&&str[i]!='w'){
			usage();
		}
	}
	return str;
}

void printQ(){
	q_print('n');
	q_print('w');
	q_print('s');
	q_print('e');
}
int 
main(int argc, char*argv[]){
   	pthread_t threads[4];
  	char* carts;
  	int i;

    carts = process(argc, argv);
  	q_init();
  	monitor_init();

  	for (i = 0; i < strlen(carts); i++){
      	q_putCart(carts[i]);
    }
    printQ();
  	for(i=0;i<4;i++){
  		int *arg=malloc(sizeof(*arg));
  		*arg=i;
	  	if((pthread_create(&threads[i], NULL, start, arg)) != 0){
	      	fprintf(stderr, "%s Thread create failed!\n", directions[i]);
	      	exit(EXIT_FAILURE);
	    }	
  	}
  	for(i=0;i<4;i++){
	  	if (pthread_join(threads[i], NULL) != 0){
	      	fprintf(stderr, "%s Thread join failed!\n", directions[i]);
	      	exit(EXIT_FAILURE);
	    }	
  	}
  q_shutdown();
  monitor_shutdown();
  return 0;
}