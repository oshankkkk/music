#include <cjson/cJSON.h>
#include "./response.h"
#include <stdio.h>
#include <string.h>
#include "../models/app.h"
#include "../player/player.h"
#include "../player/mpv/mpv.h"
#include <stddef.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

void dispatch(App *app,const char *method, cJSON *params, char *id) {
	queue *mq=app->msgqueue;
	int err=0;
	if (strcmp(method, "playSong") == 0){
		cJSON *songName = cJSON_GetObjectItemCaseSensitive(params, "songName");
		if (cJSON_IsNumber(songName)) {
 			rpcerror(-32602, "Invalid params: no numbers only song names ", id,mq);
		}

		err=playSong(app,songName->valuestring);
		if (err!=0){
			perror("rpc start song");
		}
		cJSON *song=cJSON_CreateObject();	
		cJSON_AddStringToObject(song, "id", app->currentsong->id);
		cJSON_AddStringToObject(song, "title", app->currentsong->title);
		cJSON_AddStringToObject(song, "artist", app->currentsong->title);
		cJSON_AddStringToObject(song, "duration", app->currentsong->title);
		//i dont know how to represent uploadDate and viewcount(youtube stats are not the song stats)
		cJSON_AddStringToObject(song, "artist", app->currentsong->title);
		cJSON_AddBoolToObject(song, "artist", app->currentsong->isliked);
		cJSON_AddNumberToObject(song, "artist", app->currentsong->personalplaycount);

	}
	else if (strcmp(method, "player-pause") == 0) {

		if (mpvwrite(app->mpvfd, "pause",id) < 0) {
			 rpcerror(-32602, "socket write went wrong", id,mq);
		}

	}
	else if (strcmp(method, "player-resume") == 0) {

		if (mpvwrite(app->mpvfd, "resume",id) < 0) {
			 rpcerror(-32602, "socket write went wrong", id,mq);
		}

	}
	else if (strcmp(method, "player-toggle-pause") == 0) {

		if (mpvwrite(app->mpvfd, "toggle_pause",id) < 0) {
			 rpcerror(-32602, "socket write went wrong", id,mq);
		}

	}
	else if (strcmp(method, "player-stop") == 0) {

		if (mpvwrite(app->mpvfd, "stop",id) < 0) {
			 rpcerror(-32602, "socket write went wrong", id,mq);
		}

	}
	else if (strcmp(method, "player-seek-forward") == 0) {

		if (mpvwrite(app->mpvfd, "seek_forward",id) < 0) {
			 rpcerror(-32602, "socket write went wrong", id,mq);
		}

	}
	else if (strcmp(method, "player-seek-backward") == 0) {

		if (mpvwrite(app->mpvfd, "seek_backward",id) < 0) {
			 rpcerror(-32602, "socket write went wrong", id,mq);
		}

	}
	else if (strcmp(method, "player-next") == 0) {

		if (mpvwrite(app->mpvfd, "next",id) < 0) {
			 rpcerror(-32602, "socket write went wrong", id,mq);
		}

	}
	else if (strcmp(method, "player-previous") == 0) {

		if (mpvwrite(app->mpvfd, "previous",id) < 0) {
			 rpcerror(-32602, "socket write went wrong", id,mq);
		}

	}
	else if (strcmp(method, "player-fast-forward") == 0) {

		if (mpvwrite(app->mpvfd, "fast_forward",id) < 0) {
			 rpcerror(-32602, "socket write went wrong", id,mq);
		}

	}
	else if (strcmp(method, "player-rewind") == 0) {

		if (mpvwrite(app->mpvfd, "rewind",id) < 0) {
			 rpcerror(-32602, "socket write went wrong", id,mq);
		}

	}
	else if (strcmp(method, "player-volume-up") == 0) {

		if (mpvwrite(app->mpvfd, "volume_up",id) < 0) {
			 rpcerror(-32602, "socket write went wrong", id,mq);
		}

	}
	else if (strcmp(method, "player-volume-down") == 0) {

		if (mpvwrite(app->mpvfd, "volume_down",id) < 0) {
			 rpcerror(-32602, "socket write went wrong", id,mq);
		}

	}
	else if (strcmp(method, "player-mute") == 0) {

		if (mpvwrite(app->mpvfd, "mute",id) < 0) {
			 rpcerror(-32602, "socket write went wrong", id,mq);
		}

	}
	else if (strcmp(method, "player-unmute") == 0) {

		if (mpvwrite(app->mpvfd, "unmute",id) < 0) {
			 rpcerror(-32602, "socket write went wrong", id,mq);
		}

	}
	else if (strcmp(method, "player-toggle-mute") == 0) {

		if (mpvwrite(app->mpvfd, "toggle_mute",id) < 0) {
			 rpcerror(-32602, "socket write went wrong", id,mq);
		}

	}
	else if (strcmp(method, "player-toggle-repeat") == 0) {

		if (mpvwrite(app->mpvfd, "toggle_repeat",id) < 0) {
			 rpcerror(-32602, "socket write went wrong", id,mq);
		}
	
	}else{
		 rpcerror(-32601, "Method not found", id,mq);
	}

	// dispatcher just writes stuff and response functions sends them.
	// reader called in the main as well as the tui socket writer.
}

void handler(App *app,const char *raw) {

	cJSON *req = cJSON_Parse(raw);
	if (!req) {
		rpcerror(-32700, "Parse error", NULL,app->msgqueue);
	} else {
		cJSON *method = cJSON_GetObjectItemCaseSensitive(req, "method");
		cJSON *params = cJSON_GetObjectItemCaseSensitive(req, "params");
		cJSON *id = cJSON_GetObjectItemCaseSensitive(req, "id");
		if (!cJSON_IsString(method)) {
			 rpcerror(-32600, "Invalid Request: missing 'method'", id->valuestring,app->msgqueue);
	;
		} else {
			dispatch(app,method->valuestring, params, id->valuestring);
		}
		cJSON_Delete(req);
	}

}

