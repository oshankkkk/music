#include <cjson/cJSON.h>
#include "./response.c"
#include <stdio.h>
#include <string.h>
#include "../models/app.h"
#include "../player/player.c"
static cJSON *dispatch(App *app,const char *method, cJSON *params, cJSON *id) {

	int err=0;

	if (strcmp(method, "playSong") == 0){
		cJSON *songName = cJSON_GetObjectItemCaseSensitive(params, "songName");
		if (cJSON_IsNumber(songName)) {
			return errorResponse(-32602, "Invalid params: no numbers only song names ", id);
		}

		err=playSong(app,songName->valuestring);
		if (err!=0){
			perror("rpc start song");
		}
		cJSON* response=mpvread(app->mpvfd);
		cJSON *error = cJSON_GetObjectItemCaseSensitive(response, "error");
		if (strcmp(error->valuestring, "success") == 0) {

			cJSON_Delete(response);
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

		}else{
			return resultResponse(response,id);
		}

	}

	else if (strcmp(method, "player-pause") == 0) {

		if (mpvwrite(app->mpvfd, "pause") != 0) {
			return errorResponse(-32602, "socket write went wrong", id);
		}

		return resultResponse(mpvread(app->mpvfd),id);
	}
	else if (strcmp(method, "player-resume") == 0) {

		if (mpvwrite(app->mpvfd, "resume") != 0) {
			return errorResponse(-32602, "socket write went wrong", id);
		}

		return resultResponse(mpvread(app->mpvfd),id);
	}
	else if (strcmp(method, "player-toggle-pause") == 0) {

		if (mpvwrite(app->mpvfd, "toggle_pause") != 0) {
			return errorResponse(-32602, "socket write went wrong", id);
		}

		return resultResponse(mpvread(app->mpvfd),id);
	}
	else if (strcmp(method, "player-stop") == 0) {

		if (mpvwrite(app->mpvfd, "stop") != 0) {
			return errorResponse(-32602, "socket write went wrong", id);
		}

		return resultResponse(mpvread(app->mpvfd),id);
	}
	else if (strcmp(method, "player-seek-forward") == 0) {

		if (mpvwrite(app->mpvfd, "seek_forward") != 0) {
			return errorResponse(-32602, "socket write went wrong", id);
		}

		return resultResponse(mpvread(app->mpvfd),id);
	}
	else if (strcmp(method, "player-seek-backward") == 0) {

		if (mpvwrite(app->mpvfd, "seek_backward") != 0) {
			return errorResponse(-32602, "socket write went wrong", id);
		}

		return resultResponse(mpvread(app->mpvfd),id);
	}
	else if (strcmp(method, "player-next") == 0) {

		if (mpvwrite(app->mpvfd, "next") != 0) {
			return errorResponse(-32602, "socket write went wrong", id);
		}

		return resultResponse(mpvread(app->mpvfd),id);
	}
	else if (strcmp(method, "player-previous") == 0) {

		if (mpvwrite(app->mpvfd, "previous") != 0) {
			return errorResponse(-32602, "socket write went wrong", id);
		}

		return resultResponse(mpvread(app->mpvfd),id);
	}
	else if (strcmp(method, "player-fast-forward") == 0) {

		if (mpvwrite(app->mpvfd, "fast_forward") != 0) {
			return errorResponse(-32602, "socket write went wrong", id);
		}

		return resultResponse(mpvread(app->mpvfd),id);
	}
	else if (strcmp(method, "player-rewind") == 0) {

		if (mpvwrite(app->mpvfd, "rewind") != 0) {
			return errorResponse(-32602, "socket write went wrong", id);
		}

		return resultResponse(mpvread(app->mpvfd),id);
	}
	else if (strcmp(method, "player-volume-up") == 0) {

		if (mpvwrite(app->mpvfd, "volume_up") != 0) {
			return errorResponse(-32602, "socket write went wrong", id);
		}

		return resultResponse(mpvread(app->mpvfd),id);
	}
	else if (strcmp(method, "player-volume-down") == 0) {

		if (mpvwrite(app->mpvfd, "volume_down") != 0) {
			return errorResponse(-32602, "socket write went wrong", id);
		}

		return resultResponse(mpvread(app->mpvfd),id);
	}
	else if (strcmp(method, "player-mute") == 0) {

		if (mpvwrite(app->mpvfd, "mute") != 0) {
			return errorResponse(-32602, "socket write went wrong", id);
		}

		return resultResponse(mpvread(app->mpvfd),id);
	}
	else if (strcmp(method, "player-unmute") == 0) {

		if (mpvwrite(app->mpvfd, "unmute") != 0) {
			return errorResponse(-32602, "socket write went wrong", id);
		}

		return resultResponse(mpvread(app->mpvfd),id);
	}
	else if (strcmp(method, "player-toggle-mute") == 0) {

		if (mpvwrite(app->mpvfd, "toggle_mute") != 0) {
			return errorResponse(-32602, "socket write went wrong", id);
		}

		return resultResponse(mpvread(app->mpvfd),id);
	}
	// if sending failed:
	return errorResponse(-32601, "Method not found", id);

}

