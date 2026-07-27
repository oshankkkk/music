#include <cjson/cJSON.h>
#include "./response.c"
#include <stdio.h>
#include <string.h>
#include "../models/app.h"
#include "../player/player.c"
static cJSON *dispatch(App *app,const char *method, cJSON *params, cJSON *id) {
	int err;
    if (strcmp(method, "initPlay") == 0) {
        cJSON *songName = cJSON_GetObjectItemCaseSensitive(params, "songName");
        if (cJSON_IsNumber(songName)) {
            return errorResponse(-32602, "Invalid params: no numbers only song names ", id);
        }
		err=playSong(app,songName->valuestring);
		if (err!=0){
			perror("rpc start song");
		}
        return resultResponse(cJSON_CreateString("song started"), id);
    }

    return errorResponse(-32601, "Method not found", id);
}
