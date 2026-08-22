#include <sqlite3.h>
#include <cjson/cJSON.h>
#include <stdio.h>
#include <string.h>
#include "../models/app.h"
#include <stddef.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

int addtoqueue(SongQueue *q, char *songid){
	if (q->capacity == 0) {
		q->capacity = 16;
		q->queue = malloc(q->capacity * sizeof(*q->queue));
	} else if (q->count >= q->capacity){
		size_t newcap = q->capacity * 2;
		char **tmp = realloc(q->queue, newcap * sizeof(*q->queue));
		if (tmp == NULL){
			return -1;
		}
		q->queue = tmp;
		q->capacity = newcap;
	} 
	q->queue[q->count] = strdup(songid);
	int queue_id = q->count;
	q->count++;
	return queue_id;
}

int removesongfromqueue(SongQueue *q,int index ){
	if (index < 0 || index >= q->count) return -1;
	free(q->queue[index]);
	for (int i=index;i<q->count-1;i++){
		q->queue[i]=q->queue[i+1];
	}		
	q->count--;
	return 0;
}

int clearqueue(SongQueue *q){
	if (q) {
		for (int i=0; i<q->count; i++){
			free(q->queue[i]);
		}
		q->count = 0;
	}
	return 0;
}





