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
#include "../lib/playlists.h"
#include "../lib/queue.h"

void playerhandler(App *app, char *method, cJSON *params, int id, queue *mq);

void dispatch(App *app,char *method,char *type, cJSON *params, int id) {
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
		
		if (app->currentsong != NULL) {
			cJSON *song=cJSON_CreateObject();	
			cJSON_AddStringToObject(song, "songid", app->currentsong->id);
			cJSON_AddStringToObject(song, "title", app->currentsong->title);
			cJSON_AddStringToObject(song, "artist", app->currentsong->artist);
			cJSON_AddNumberToObject(song, "duration", app->currentsong->duration);
			cJSON_AddBoolToObject(song, "isliked", app->currentsong->isliked);
			cJSON_AddNumberToObject(song, "personalplaycount", app->currentsong->personalplaycount);

			cmdresponse(song,app->msgqueue,"song");
		} else {
			rpcerror(-32603, "Internal error: currentsong is NULL", id, mq);
		}
	}else if(strcmp(type, "player")){
		playerhandler(app,method,params,id,app->msgqueue);
	}else if (strcmp(type,"lib")){
		libhandler(app,method,params,id);
	}else if (strcmp(type,"queue")){
		queuehandler(app,method,params,id);
	}else{
		 rpcerror(-32601, "Method not found", id,mq);
	}
}

void playerhandler(App *app,char *method,cJSON *params, int id, queue *mq){

	if (strcmp(method, "player-pause") == 0) {

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

	}

}

void handler(App *app,const char *raw) {

	cJSON *req = cJSON_Parse(raw);
	if (!req) {
		rpcerror(-32700, "Parse error", -1,app->msgqueue);
	} else {
		cJSON *type = cJSON_GetObjectItemCaseSensitive(req, "type");
		cJSON *method = cJSON_GetObjectItemCaseSensitive(req, "method");
		cJSON *params = cJSON_GetObjectItemCaseSensitive(req, "params");
		cJSON *id = cJSON_GetObjectItemCaseSensitive(req, "id");
		if (!cJSON_IsString(method)) {
			 rpcerror(-32600, "Invalid Request: missing 'method'", id->valueint,app->msgqueue);
	;
		} else {
			dispatch(app,method->valuestring,type->valuestring, params, id->valueint);
		}
		cJSON_Delete(req);
	}
}

