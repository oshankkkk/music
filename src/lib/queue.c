#include <sqlite3.h>
#include <cjson/cJSON.h>
//#include "./response.h"
#include <stdio.h>
#include <string.h>
#include "../models/app.h"
#include "../player/mpv/mpv.h"
#include <stddef.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

int addtoqueue(SongQueue *q, int songid){
	if (q->capacity == 0) {
		q->capacity = 16;
		q->queue = malloc(q->capacity * sizeof(*q->queue));
	} else if (q->count >= q->capacity){
		size_t newcap = q->capacity * 2;
		int *tmp = realloc(q->queue, newcap * sizeof(*q->queue));
		if (tmp == NULL){
			return -1;
		}
		q->queue = tmp;
		q->capacity = newcap;
	} 
	q->queue[q->count] = songid;
	int queue_id = q->count;
	q->count++;
	return queue_id;
}

int removesongfromqueue(SongQueue *q,int index ){
	if (index < 0 || index >= q->count) return -1;
	for (int i=index;i<q->count-1;i++){
		q->queue[i]=q->queue[i+1];
	}		
	q->count--;
	return 0;
}

int clearqueue(SongQueue *q){
	if (q) {
		q->count = 0;
	}
	return 0;
}

int queuehandler(App *app, char *method, cJSON *params, int id) {
    int success = 0;
    cJSON *resp = cJSON_CreateObject();
    cJSON_AddNumberToObject(resp, "id", id);
    cJSON_AddStringToObject(resp, "method", method);

    if (strcmp(method, "lib-addtoqueue") == 0) {
        cJSON *songid_node = cJSON_GetObjectItemCaseSensitive(params, "songid");
        if (songid_node) {
            int qid = addtoqueue(app->songqueue, songid_node->valueint);
            if (qid >= 0) {
                success = 1;
                cJSON_AddNumberToObject(resp, "queueid", qid);
            }
        }
    } else if (strcmp(method, "lib-removefromqueue") == 0) {
        cJSON *songid_node = cJSON_GetObjectItemCaseSensitive(params, "songid");
        if (songid_node) {
            if (removesongfromqueue(app->songqueue, songid_node->valueint) == 0) {
                success = 1;
            }
        }
    } else if (strcmp(method, "lib-clearqueue") == 0) {
        if (clearqueue(app->songqueue) == 0) {
            success = 1;
        }
    }

    if (success) {
        cJSON_AddTrueToObject(resp, "success");
    } else {
        cJSON_AddFalseToObject(resp, "success");
    }
    cmdresponse(resp, app->msgqueue,"queue");
    return success ? 0 : -1;
}

