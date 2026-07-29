#include <cjson/cJSON.h>
#include "./response.c"
#include <stdio.h>
#include <string.h>
#include "../models/app.h"
#include "../player/player.c"
static cJSON *dispatch(App *app,const char *method, cJSON *params, cJSON *id) {
	int err;
    if (strcmp(method, "playSong") == 0) {
        cJSON *songName = cJSON_GetObjectItemCaseSensitive(params, "songName");
        if (cJSON_IsNumber(songName)) {
            return errorResponse(-32602, "Invalid params: no numbers only song names ", id);
        }
		printf("playsong");
		err=playSong(app,songName->valuestring);
		if (err!=0){
			perror("rpc start song");
		}
		
		//the song object with the meta data to show to the user 
		//how does we sync the mpv player to the tui 
		//We can do the buttons through the ipc, but the song duration.
		//Response: {"jsonrpc":"2.0","result":5,"id":1}

		cJSON *song=cJSON_CreateObject();	
    	cJSON_AddStringToObject(song, "id", app->currentsong->id);
    	cJSON_AddStringToObject(song, "title", app->currentsong->title);
    	cJSON_AddStringToObject(song, "artist", app->currentsong->title);
    	cJSON_AddStringToObject(song, "duration", app->currentsong->title);
		//i dont know how to represent uploadDate and viewcount(youtube stats are not the song stats)
    	cJSON_AddStringToObject(song, "artist", app->currentsong->title);
    	cJSON_AddBoolToObject(song, "artist", app->currentsong->isliked);
    	cJSON_AddNumberToObject(song, "artist", app->currentsong->personalplaycount);

        return resultResponse(song, id);
    }

    return errorResponse(-32601, "Method not found", id);

}
