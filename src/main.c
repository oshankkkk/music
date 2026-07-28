#include <cjson/cJSON.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "./rpc/handler.c"
#include "./models/app.h"
//#include "./player/player.c"

#include <stddef.h>

int startup(App *app){
	sqlite3 *db =InitDb();
	if (!db){
		perror("db init");
		return 1; 
	}else{
		printf("db works\n");
		app->db=db;
	}

	sqlite3 *cache = InitCache();
	if (!cache){
		perror("cache db init");
		return 1;
	}else{
		printf("cache db works\n");
		app->cache=cache;
	}
	Song *song=NULL;
	app->currentsong=song;

	return 0;
}

int main(void) {
	//char *songName;
	App app;
	//char *path;
	int err=0;
	char *socketpath = "./build/us.socket";

	err = startup(&app);
    if (err != 0) {
        perror("startup");
        goto cleanup;
    }

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
	}
	printf("RPC server listening on %s\n",socketpath);

	while (1) {
		int clientFd=accept(serverFd,NULL,NULL);	
		if (clientFd< 0) {
			perror("accept");
			continue;
		}	
		char *buf=malloc(5053);	
		read(clientFd,buf,5051);

		buf[5052]='\0';

		char *response = handler(&app,buf);
		size_t len = strlen(response);

		write(clientFd, response, len);
		write(clientFd, "\n", 1);

		free(response);

		close(clientFd);
	}


cleanup:
    if (app.db)    sqlite3_close(app.db);
    if (app.cache) sqlite3_close(app.cache);
	unlink(socketpath);
    return err;
}


