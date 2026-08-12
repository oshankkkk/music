#ifndef MPV_H
#define MPV_H

#include <cjson/cJSON.h>
#include "../../models/app.h"

int mpvstart(void);
int mpvinit(char *socketpath);
int mpvplay(int ipcfd,char *path);
int mpvsongtimer(int ipcfd,int observer_id, int id);
int mpvwrite(int ipcfd, char *action, int id);
void eventresponse(cJSON *response,queue *msgqueue);
void cmdresponse(cJSON *response,queue *msgqueue,char *type);
void *mpvread(void *arg);

#endif
