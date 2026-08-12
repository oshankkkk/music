#ifndef QUEUE_H
#define QUEUE_H

#include <cjson/cJSON.h>
#include "../models/app.h"
#include "../models/lib.h"

int addtoqueue(SongQueue *q, int songid);
int removesongfromqueue(SongQueue *q, int index);
int clearqueue(SongQueue *q);
int queuehandler(App *app, char *method, cJSON *params, int id);

#endif // QUEUE_H
