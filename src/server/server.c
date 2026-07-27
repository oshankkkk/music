#include <cjson/cJSON.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "./dispatch.c"
//#include "./response.c"

static char *mainHandler(const char *raw) {
    cJSON *req = cJSON_Parse(raw);
    cJSON *response;
	
    if (!req) {
        response =errorResponse(-32700, "Parse error", NULL);
    } else {
        cJSON *method = cJSON_GetObjectItemCaseSensitive(req, "method");
        cJSON *params = cJSON_GetObjectItemCaseSensitive(req, "params");
        cJSON *id = cJSON_GetObjectItemCaseSensitive(req, "id");

        if (!cJSON_IsString(method)) {
            response =errorResponse(-32600, "Invalid Request: missing 'method'", id);
        } else {
            response = dispatch(method->valuestring, params, id);
        }
        cJSON_Delete(req);
    }

    char *out = cJSON_PrintUnformatted(response);
    cJSON_Delete(response);
    return out;
}


int serverInit(void) {
	int err=0;
	char *socketpath = "./us.socket";
	int serverFd=socket(AF_UNIX,SOCK_STREAM,0);
	if (serverFd<0){
		perror("unix socket");		
		return 1;
	}	
	struct sockaddr_un addr;
	addr.sun_family=AF_UNIX;
	strncpy(addr.sun_path, socketpath, sizeof(addr.sun_path) - 1);
	unlink(socketpath);
	err=bind(serverFd,(struct sockaddr *)&addr,sizeof(addr));
	if (err < 0) {
		perror("bind");
		return 1;
	}

	if (listen(serverFd, 8) < 0) {
		perror("listen");
		return 1;

		printf("RPC server listening on %s\n",socketpath);

		while (1) {
		int clientFd=accept(serverFd,NULL,NULL);	
		    if (clientFd< 0) {
            perror("accept");
            continue;
        	}	
		char *buf;	
		read(clientFd,buf,sizeof(*buf)-1);
		buf[sizeof(buf)]='\0';
			

            char *response = mainHandler(buf);
            size_t len = strlen(response);

            // Write response followed by a newline delimiter.
            write(clientFd, response, len);
            write(clientFd, "\n", 1);

            free(response);

        close(clientFd);
        }

    }
	unlink(socketpath);
		return 0;

}

int main(void) {
	serverInit();
	return 0;
}
