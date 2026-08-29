#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "../../models/app.h"
#include "../../models/msg.h"
#include "../../rpc/msg.h"
#include <cjson/cJSON.h>

int mpvstart(){
	pid_t pid = fork();
	if (pid == 0) {
		execlp(
				"mpv",
				"mpv",
				"--idle=yes",
				"--no-video",
				"--input-ipc-server=./build/mpv.socket",
				NULL
			  );

		perror("execlp");
		_exit(1);
	}
	else if (pid > 0) {
		printf("mpv started with PID %d\n", pid);
		return pid;
	}
	else {
		perror("fork");
		return -1;		
	}
}
int mpvinit(char *socketpath){
	int sockfd=socket(AF_UNIX,SOCK_STREAM,0);
	if (sockfd==-1){
		perror("error");
		return -1;
	}
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path,  socketpath, sizeof(addr.sun_path)-1);

	if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {

		printf("this is the error in mpv init");
		perror("connect error");
		//	close(sockfd);
		return -1;
	}
	return sockfd;
}

int mpvplay(int ipcfd,char *path){

	char cmd[5050] = {0};
	snprintf(cmd, sizeof(cmd),"{\"command\":[\"loadfile\",\"%s\"],\"request_id\":\"1\"}\n",path);		
	snprintf(cmd+strlen(cmd),sizeof(cmd)-strlen(cmd),
			"{\"command\":[\"set_property\",\"pause\",false]}\n");
	return write(ipcfd, cmd, strlen(cmd));

}

int mpvgetDuration(int ipcfd){

char cmd[1024] = {0};
snprintf(cmd+strlen(cmd),sizeof(cmd)-strlen(cmd),
			"{\"command\":[\"get_property\",\"duration\"],\"request_id\":\"2\"}\n");
	return write(ipcfd, cmd, strlen(cmd));

}

int mpvsongtimer(int ipcfd,int observer_id,int id){

char cmd[1024] = {0};
snprintf(
    cmd,
    sizeof(cmd),
    "{\"command\":[\"observe_property\",%d,\"time-pos\"],\"request_id\":\"%d\"}\n",
    observer_id,
    id
);

return write(ipcfd, cmd, strlen(cmd));
}
int mpvwrite(int ipcfd, char *action, int id){
	char cmd[1024] = {0};

	if (strcmp(action, "pause") == 0) {
		snprintf(cmd, sizeof(cmd),
				"{\"command\":[\"set_property\",\"pause\",true],\"request_id\":\"%d\"}\n", id);
	}
	else if (strcmp(action, "resume") == 0) {
		snprintf(cmd, sizeof(cmd),
				"{\"command\":[\"set_property\",\"pause\",false],\"request_id\":\"%d\"}\n", id);

	}
	else if (strcmp(action, "toggle_pause") == 0) {
		snprintf(cmd, sizeof(cmd),
				"{\"command\":[\"cycle\",\"pause\"],\"request_id\":\"%d\"}\n", id);
	}
	else if (strcmp(action, "stop") == 0) {
		snprintf(cmd, sizeof(cmd),
				"{\"command\":[\"quit\"],\"request_id\":\"%d\"}\n", id);
	}
	else if (strcmp(action, "seek_forward") == 0) {
		snprintf(cmd, sizeof(cmd),
				"{\"command\":[\"seek\",10,\"relative\"],\"request_id\":\"%d\"}\n", id);
	}
	else if (strcmp(action, "seek_backward") == 0) {
		snprintf(cmd, sizeof(cmd),
				"{\"command\":[\"seek\",-10,\"relative\"],\"request_id\":\"%d\"}\n", id);
	}
	else if (strcmp(action, "next") == 0) {
		snprintf(cmd, sizeof(cmd),
				"{\"command\":[\"playlist-next\",\"force\"],\"request_id\":\"%d\"}\n", id);
	}
	else if (strcmp(action, "previous") == 0) {
		snprintf(cmd, sizeof(cmd),
				"{\"command\":[\"playlist-prev\",\"force\"],\"request_id\":\"%d\"}\n", id);
	}
	else if (strcmp(action, "fast_forward") == 0) {
		snprintf(cmd, sizeof(cmd),
				"{\"command\":[\"seek\",60,\"relative\"],\"request_id\":\"%d\"}\n", id);
	}
	else if (strcmp(action, "rewind") == 0) {
		snprintf(cmd, sizeof(cmd),
				"{\"command\":[\"seek\",-60,\"relative\"],\"request_id\":\"%d\"}\n", id);
	}
	else if (strcmp(action, "volume_up") == 0) {
		snprintf(cmd, sizeof(cmd),
				"{\"command\":[\"add\",\"volume\",5],\"request_id\":\"%d\"}\n", id);
	}
	else if (strcmp(action, "volume_down") == 0) {
		snprintf(cmd, sizeof(cmd),
				"{\"command\":[\"add\",\"volume\",-5],\"request_id\":\"%d\"}\n", id);
	}
	else if (strcmp(action, "mute") == 0) {
		snprintf(cmd, sizeof(cmd),
				"{\"command\":[\"set_property\",\"mute\",true],\"request_id\":\"%d\"}\n", id);
	}
	else if (strcmp(action, "unmute") == 0) {
		snprintf(cmd, sizeof(cmd),
				"{\"command\":[\"set_property\",\"mute\",false],\"request_id\":\"%d\"}\n", id);
	}
	else if (strcmp(action, "toggle_mute") == 0) {
		snprintf(cmd, sizeof(cmd),
				"{\"command\":[\"cycle\",\"mute\"],\"request_id\":\"%d\"}\n", id);
	}
	else if (strcmp(action, "toggle_repeat") == 0) {
		snprintf(cmd, sizeof(cmd),
				"{\"command\":[\"cycle\",\"loop-file\"],\"request_id\":\"%d\"}\n", id);
	}

	return write(ipcfd, cmd, strlen(cmd));
}

void eventresponse(cJSON *response,queue *msgqueue){

	cJSON *resp = cJSON_CreateObject();
	cJSON_AddStringToObject(resp, "jsonrpc", "2.0");
	cJSON_AddStringToObject(resp, "type", "mpv-event");
	cJSON_AddItemToObject(resp, "response", response);

	char *item=cJSON_PrintUnformatted(resp);

	push(msgqueue,strlen(item),item);
	free(item);
	cJSON_Delete(resp);
}

void cmdresponse(cJSON *response,queue *msgqueue,char *type){

	cJSON *resp = cJSON_CreateObject();
	cJSON_AddStringToObject(resp, "jsonrpc", "2.0");
	if (strcmp(type,"song")==0){

	cJSON_AddStringToObject(resp, "type", "song");
	}else if (strcmp(type,"mpv")==0){

	cJSON_AddStringToObject(resp, "type", "mpv-reply");
	}else if (strcmp(type,"playlist")==0){

	cJSON_AddStringToObject(resp, "type", "playlist");
	}else if (strcmp(type,"queue")==0){

	cJSON_AddStringToObject(resp, "type", "queue");
	}else{

	cJSON_AddStringToObject(resp, "type", "unknown");

	}

	cJSON_AddItemToObject(resp, "response", response);

	char *item=cJSON_PrintUnformatted(resp);

	printf("this is the the string song %s\n",item);
	push(msgqueue,strlen(item),item);
	free(item);
	cJSON_Delete(resp);
}

void *mpvread(void *arg){
	App *app=(App *)arg;
	char buff[2048];

	while (1) {
		int n = read(app->mpvfd, buff, sizeof(buff) - 1);
		if (n <= 0) {
			break;
		}

		buff[n] = '\0';

		cJSON *response = cJSON_Parse(buff);
		if (!response) {
			continue;
		}

		cJSON *error = cJSON_GetObjectItemCaseSensitive(response, "error");

		if (error==NULL){

			cJSON *event = cJSON_GetObjectItemCaseSensitive(response, "event");
			if (event==NULL){
				cJSON_Delete(response);
				continue;
			}
			if (cJSON_IsString(event) && strcmp(event->valuestring, "file-loaded") == 0) {
				mpvgetDuration(app->mpvfd);
			}
			eventresponse(response,app->msgqueue);
		} else {
			cmdresponse(response,app->msgqueue,"mpv");
		}
	}

		return NULL;
}

