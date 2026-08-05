#ifndef MSG_H
#define MSG_H

#include <pthread.h>

#define MSG_MAX      4096
#define QUEUE_CAP    64



typedef struct{
	char msg[MSG_MAX];
	size_t len;
}msg;

typedef struct{
	int head,tail,count;
	msg items[QUEUE_CAP];
	pthread_mutex_t lock;
	pthread_cond_t hybernate;
}queue ;

#endif
