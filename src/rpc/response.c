#include <cjson/cJSON.h>


static cJSON *resultResponse(cJSON *result, cJSON *id) {
    cJSON *resp = cJSON_CreateObject();
    cJSON_AddStringToObject(resp, "jsonrpc", "2.0");
    cJSON_AddItemToObject(resp, "result", result);

    // id must be echoed back exactly as received; duplicate cause the
    // request object still owns the original.
    cJSON_AddItemToObject(resp, "id", cJSON_Duplicate(id, 1));
    return resp;
}

static cJSON *errorResponse(int code, const char *message, cJSON *id) {
    cJSON *resp = cJSON_CreateObject();
    cJSON_AddStringToObject(resp, "jsonrpc", "2.0");

    cJSON *error = cJSON_CreateObject();
    cJSON_AddNumberToObject(error, "code", code);
    cJSON_AddStringToObject(error, "message", message);
    cJSON_AddItemToObject(resp, "error", error);

    cJSON_AddItemToObject(resp, "id", id ? cJSON_Duplicate(id, 1) : cJSON_CreateNull());
    return resp;
}
