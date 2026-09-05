#include <cjson/cJSON.h>
#include "./response.h"
#include <string.h>
#include "../models/app.h"
#include "../db/song.h"
#include "../player/mpv/mpv.h"
#include <stddef.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "../db/playlists.h"
#include "../lib/queue.h"

void playerhandler(App *app, char *method, cJSON *params, int id, queue *mq);

void dispatch(App *app,char *method,char *type, cJSON *params, int id) {
	queue *mq=app->msgqueue;
	if (strcmp(type, "song") == 0){
		songhandler(app,method,params,id);
	}else if(strcmp(type, "player") == 0){
		playerhandler(app,method,params,id,app->msgqueue);
	}else if (strcmp(type,"lib") == 0){
		libhandler(app,method,params,id);
	}else if (strcmp(type,"queue") == 0){
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
	else if (strcmp(method, "player-get-volume") == 0) {
		if (mpvwrite(app->mpvfd, "get_volume",id) < 0) {
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
	const char *current_ptr = raw;
	const char *return_parse_end = NULL;

	while (current_ptr != NULL && *current_ptr != '\0') {
		cJSON *req = cJSON_ParseWithOpts(current_ptr, &return_parse_end, 0);
		if (!req) {
			// If it fails to parse but there's non-whitespace content, log/error.
			// Otherwise just break.
			break;
		}

		cJSON *type = cJSON_GetObjectItemCaseSensitive(req, "type");
		cJSON *method = cJSON_GetObjectItemCaseSensitive(req, "method");
		cJSON *params = cJSON_GetObjectItemCaseSensitive(req, "params");
		cJSON *id = cJSON_GetObjectItemCaseSensitive(req, "id");
		int req_id = (id && cJSON_IsNumber(id)) ? id->valueint : -1;
		if (!cJSON_IsString(method) || !cJSON_IsString(type)) {
			rpcerror(-32600, "Invalid Request: missing 'method' or 'type'", req_id, app->msgqueue);
		} else {
			dispatch(app, method->valuestring, type->valuestring, params, req_id);
		}
		cJSON_Delete(req);

		current_ptr = return_parse_end;
	}
}

