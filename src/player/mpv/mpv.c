#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
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
//			int status;
			//waitpid(pid, &status, 0);
		}
		else {
			perror("fork");
			return 1;		
		}
	
	return 0;
}

int mpvinit(char *socketpath){
	int sockfd=socket(AF_UNIX,SOCK_STREAM,0);
	if (sockfd==-1){
		perror("error");
	}
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path,  socketpath, sizeof(addr.sun_path)-1);

	if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("connect");
		//	close(sockfd);
		return -1;
	}
	return sockfd;
}
int mpvplay(int ipcfd,char *path){

	char cmd[1024] = {0};
	snprintf(cmd, sizeof(cmd),"{\"command\":[\"loadfile\",\"%s\"]}\n",path);		
	
	return write(ipcfd, cmd, strlen(cmd));

}

int mpvwrite(int ipcfd, char *action)
{
	char cmd[1024] = {0};
	if (strcmp(action, "pause") == 0) {
		strcpy(cmd, "{\"command\":[\"set_property\",\"pause\",true]}\n");
	}
	else if (strcmp(action, "resume") == 0) {
		strcpy(cmd, "{\"command\":[\"set_property\",\"pause\",false]}\n");
	}
	else if (strcmp(action, "toggle_pause") == 0) {
		strcpy(cmd, "{\"command\":[\"cycle\",\"pause\"]}\n");
	}
	else if (strcmp(action, "stop") == 0) {
		strcpy(cmd, "{\"command\":[\"quit\"]}\n");
	}
	else if (strcmp(action, "seek_forward") == 0) {
		strcpy(cmd, "{\"command\":[\"seek\",10,\"relative\"]}\n");
	}
	else if (strcmp(action, "seek_backward") == 0) {
		strcpy(cmd, "{\"command\":[\"seek\",-10,\"relative\"]}\n");
	}
	else if (strcmp(action, "next") == 0) {
		strcpy(cmd, "{\"command\":[\"playlist-next\",\"force\"]}\n");
	}
	else if (strcmp(action, "previous") == 0) {
		strcpy(cmd, "{\"command\":[\"playlist-prev\",\"force\"]}\n");
	}
	else if (strcmp(action, "fast_forward") == 0) {
		strcpy(cmd, "{\"command\":[\"seek\",60,\"relative\"]}\n");
	}
	else if (strcmp(action, "rewind") == 0) {
		strcpy(cmd, "{\"command\":[\"seek\",-60,\"relative\"]}\n");
	}
	else if (strcmp(action, "volume_up") == 0) {
		strcpy(cmd, "{\"command\":[\"add\",\"volume\",5]}\n");
	}
	else if (strcmp(action, "volume_down") == 0) {
		strcpy(cmd, "{\"command\":[\"add\",\"volume\",-5]}\n");
	}
	else if (strcmp(action, "mute") == 0) {
		strcpy(cmd, "{\"command\":[\"set_property\",\"mute\",true]}\n");
	}
	else if (strcmp(action, "unmute") == 0) {
		strcpy(cmd, "{\"command\":[\"set_property\",\"mute\",false]}\n");
	}
	else if (strcmp(action, "toggle_mute") == 0) {
		strcpy(cmd, "{\"command\":[\"cycle\",\"mute\"]}\n");
	}


	return	write(ipcfd, cmd, strlen(cmd));
	//mpvread(ipcfd);
}

cJSON *mpvread(int fd){
    char buff[2048];

    int n = read(fd, buff, sizeof(buff) - 1);
    if (n < 0) {
        return NULL;
    }

    buff[n] = '\0';

    cJSON *response = cJSON_Parse(buff);
    if (!response) {
        return NULL;
    }

    cJSON *error = cJSON_GetObjectItemCaseSensitive(response, "error");
    if (error == NULL || !cJSON_IsString(error)) {
        cJSON_Delete(response);
        return NULL;
    }

    // mpv command succeeded
   // if (strcmp(error->valuestring, "success") == 0) {
   //     cJSON_Delete(response);
   //     return NULL;
   // }
   //
    // mpv returned an error
    return response;
}

//cJSON *event = cJSON_GetObjectItemCaseSensitive(response, "event");
		//if (event==NULL){
		//	return 1;
		//}
		// what ever that the event response has to parse
		// The event repsonse has different values in different types ryt, how do we handle that
		//
