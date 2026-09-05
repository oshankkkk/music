#ifndef MSG_H
#define MSG_H

#include <pthread.h>

#define QUEUE_CAP    64

typedef struct{
	char *msg;
	size_t len;
}msg;

typedef struct{
	int head,tail,count;
	msg items[QUEUE_CAP];
	pthread_mutex_t lock;
	pthread_cond_t hybernate;
}queue ;

#endif
