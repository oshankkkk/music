#ifndef RPC_MSG_H
#define RPC_MSG_H

#include <stddef.h>
#include "../models/msg.h"

void push(queue *queue,size_t itemlen,char *value);
int pop(queue *q, msg *out,int timeout_ms);

#endif
