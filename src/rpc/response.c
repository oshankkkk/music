#include <cjson/cJSON.h>
#include <unistd.h>
#include "./msg.h"

void *rpcerror(int code, const char *message, char* id,queue *msgqueue) {
    cJSON *resp = cJSON_CreateObject();
    cJSON_AddStringToObject(resp, "jsonrpc", "2.0");

    cJSON *error = cJSON_CreateObject();
    cJSON_AddNumberToObject(error, "code", code);
    cJSON_AddStringToObject(error, "message", message);
    cJSON_AddItemToObject(resp, "error", error);

    cJSON_AddStringToObject(resp, "id",id);

	char *item=cJSON_PrintUnformatted(resp);
	push(msgqueue,sizeof(item),item);

	return NULL;
}
