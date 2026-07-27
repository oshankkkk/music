#include <cjson/cJSON.h>
#include "./response.c"
#include <string.h>

static cJSON *dispatch(const char *method, cJSON *params, cJSON *id) {
    if (strcmp(method, "add") == 0) {
        cJSON *a = cJSON_GetObjectItemCaseSensitive(params, "a");
        cJSON *b = cJSON_GetObjectItemCaseSensitive(params, "b");
        if (!cJSON_IsNumber(a) || !cJSON_IsNumber(b)) {
            return errorResponse(-32602, "Invalid params: expected numeric 'a' and 'b'", id);
        }

        //double sum = add(a->valuedouble, b->valuedouble);
        //return response(cJSON_CreateNumber(sum), id);
    }

    // Unknown method -> JSON-RPC standard error code -32601
    return errorResponse(-32601, "Method not found", id);
}
