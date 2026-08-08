#include <cjson/cJSON.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>
#include "./models/app.h"
#include "./rpc/dispatch.h"
#include "./rpc/msg.h"
#include "./player/mpv/mpv.h"
#include "./db/song.h"
#include "./db/cache/cache.h"

#define TUISOCK_PATH "./build/us.socket"
#define MPVSOCK_PATH "./build/mpv.socket"
#define MSG_MAX      4096
#define QUEUE_CAP    64

int startup(App *app){

	if(mpvstart()){
		sleep(1);
		return -1;
	}

	int mpvfd=mpvinit(MPVSOCK_PATH);
	if (mpvfd<0){
		printf("mpv not set");
		return -1;
	}else{
		printf("mpv socket works\n");
		app->mpvfd=mpvfd;
	}

	sqlite3 *db =InitDb();
	if (!db){
		perror("db init");
		return -1; 
	}else{
		printf("db works\n");
		app->db=db;
	}

	sqlite3 *cache = InitCache();

	if (!cache){
		perror("cache db init");
		return -1;
	}else{
		printf("cache db works\n");
		app->cache=cache;
	}

	Song *song=NULL;
	app->currentsong=song;
	mpvsongtimer(app->mpvfd,0,0);
	return 0;
}

void *tuiwriter(void *arg){

	App *app=(App *)arg;
	msg msg;
	uint32_t len;

	while(1){
		if(pop(app->msgqueue,&msg, 200)){
			if (app->clientfd > 0) {
				len = (uint32_t)msg.len;
				write(app->clientfd, &len, sizeof(len));
				write(app->clientfd, msg.msg, msg.len);
			}
		}
	}
	//while (1) {
	//	int clientFd=accept(app->serverfd,NULL,NULL);	
	//	app->clientfd=clientFd;
	//	if (clientFd< 0) {
	//		printf("prob here\n");
	//		perror("accept");
	//		continue;
	//	}	
	//	char *buf=malloc(6063);	
	//	size_t n=read(clientFd,buf,5051);
	//	if (n>0){
	//		buf[n]='\0';
	//	}else{
	//		buf[0]='\0';
	//	}
	//	handler(app,buf);
	//}
	
}

queue msgqueue={
	.head=0,
	.tail=0,
	.count=0,
	.lock=PTHREAD_MUTEX_INITIALIZER,
	.hybernate=PTHREAD_COND_INITIALIZER,
};

int main(void) {

	pthread_t tui,mpvreader;
	App app;
	memset(&app, 0, sizeof(app));
	int err=0;
	
	err = startup(&app);
	if (err < 0) {
		perror("startup");
		goto cleanup;
	}
	int serverFd=socket(AF_UNIX,SOCK_STREAM,0);
	app.serverfd=serverFd;

	printf("this is the fd in main %d",app.serverfd);
	if (serverFd<0){
		perror("unix socket");		
		goto cleanup;
	}	

	app.msgqueue=&msgqueue;

	struct sockaddr_un addr;
	addr.sun_family=AF_UNIX;
	strncpy(addr.sun_path, TUISOCK_PATH, sizeof(addr.sun_path) - 1);
	unlink(TUISOCK_PATH);
	err=bind(serverFd,(struct sockaddr *)&addr,sizeof(addr));
	if (err < 0) {
		perror("bind");
		goto cleanup;
	}
	if (listen(serverFd, 8) < 0) {
		perror("listen");
		goto cleanup;
	}

	printf("RPC server listening on %s\n",TUISOCK_PATH);

	if (pthread_create(&tui, NULL,tuiwriter, &app) != 0) {
		perror("pthread_create funcA");
		goto cleanup;
	}
	if (pthread_create(&mpvreader, NULL, mpvread, &app) != 0) {
		perror("pthread_create funcB");
		goto cleanup;
	}

	while (1) {
		int clientFd=accept(app.serverfd,NULL,NULL);	
		app.clientfd=clientFd;
		if (clientFd< 0) {
			printf("prob here\n");
			perror("accept");
			continue;
		}	
		while (1) {
			char *buf=malloc(6063);	
			if (!buf) break;
			ssize_t n=read(clientFd,buf,5051);
			if (n>0){
				buf[n]='\0';
				handler(&app,buf);
				free(buf);
			}else{
				free(buf);
				break;
			}
		}
		close(clientFd);
	}

	pthread_join(tui, NULL);
	pthread_join(mpvreader, NULL);
	return 0;

cleanup:
	if (app.db)    sqlite3_close(app.db);
	if (app.cache) sqlite3_close(app.cache);
	unlink(TUISOCK_PATH);
	unlink(MPVSOCK_PATH);

	return err;
}

