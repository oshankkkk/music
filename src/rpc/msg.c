#include <cjson/cJSON.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>
#include "../models/msg.h"

#define QUEUE_CAP    64

void push(queue *queue,size_t itemlen,char *value){
	pthread_mutex_lock(&queue->lock);
	if(queue->count==QUEUE_CAP){
		free(queue->items[queue->head].msg);
		queue->head=(queue->head+1)%QUEUE_CAP;
		queue->count--;
	}
	msg *item=&queue->items[queue->tail];
	item->msg = malloc(itemlen);
	memcpy(item->msg, value, itemlen);	
	item->len=itemlen;
	queue->tail=(queue->tail+1)%QUEUE_CAP;
	queue->count++;
	pthread_cond_signal(&queue->hybernate);
	pthread_mutex_unlock(&queue->lock);

}

int pop(queue *q, msg *out,int timeout_ms){
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec  += timeout_ms / 1000;
	ts.tv_nsec += (long)(timeout_ms % 1000) * 1000000L;
	if (ts.tv_nsec >= 1000000000L) { ts.tv_sec++; ts.tv_nsec -= 1000000000L; }

	pthread_mutex_lock(&q->lock);
	while(q->count == 0){
		if(pthread_cond_timedwait(&q->hybernate, &q->lock,&ts)==ETIMEDOUT){
			pthread_mutex_unlock(&q->lock);
			return 0;
		}
	}
	*out = q->items[q->head];

	q->head = (q->head + 1) %QUEUE_CAP;
	q->count--;
	pthread_mutex_unlock(&q->lock);
	return 1;
}


