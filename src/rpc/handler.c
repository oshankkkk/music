#include <cjson/cJSON.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "./dispatch.c"

static char *handler(App *app,const char *raw) {
    cJSON *req = cJSON_Parse(raw);
    cJSON *response;
	
    if (!req) {
        response =errorResponse(-32700, "Parse error", NULL);
    } else {
        cJSON *method = cJSON_GetObjectItemCaseSensitive(req, "method");
        cJSON *params = cJSON_GetObjectItemCaseSensitive(req, "params");
        cJSON *id = cJSON_GetObjectItemCaseSensitive(req, "id");
		printf("handler");
        if (!cJSON_IsString(method)) {
            response =errorResponse(-32600, "Invalid Request: missing 'method'", id);
        } else {

		printf("dispatch");
            response = dispatch(app,method->valuestring, params, id);

        }
        cJSON_Delete(req);
    }

    char *out = cJSON_PrintUnformatted(response);

		// printf("==================================");
		// printf("==================================");
		// printf("==================================");
		// printf("here is the response %s",out);
		//
		printf("here is the response %s\n", out);
fflush(stdout);
    cJSON_Delete(response);
    return out;
}


