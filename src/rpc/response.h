#ifndef RPC_RESPONSE_H
#define RPC_RESPONSE_H

#include <cjson/cJSON.h>
#include "../models/msg.h"

void *rpcerror(int code, const char *message, char* id,queue *msgqueue);

#endif
