#ifndef RPC_DISPATCH_H
#define RPC_DISPATCH_H

#include <cjson/cJSON.h>
#include "../models/app.h"

void dispatch(App *app,const char *method, cJSON *params, char *id);
void handler(App *app,const char *raw);

#endif
