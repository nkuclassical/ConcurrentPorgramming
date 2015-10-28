#ifndef _MONITOR_
#define _MONITOR_


pthread_mutex_t mutex;
pthread_cond_t ready, crossing;
struct cart_t *readyCart, *crossingCart;

int next=1;

extern void monitor_init();
extern void monitor_arrive(struct cart_t*);
extern void monitor_cross(struct cart_t*);
extern void monitor_leave(struct cart_t*);
extern void monitor_shutdown();

#endif