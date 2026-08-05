#include <stdio.h>
#include <sys/wait.h>
#include <strings.h>
#include <unistd.h>
#include <stdbool.h>
#include <cjson/cJSON.h>
#include <sqlite3.h>
#include "../db/cache/cache.h"
#include "../yt/yt.h"
//#include "../models/app.h"
//#include "./server/server.c"

int backgroundCaching(Song *song, sqlite3 *cache) {

	printf("\n the song is caching \n");
	time_t expires = song->playedTime+ (7 * 24 * 60 * 60);
	pid_t pid = fork();
	if (pid == 0) {
		if (fork() == 0) {
			char filepath[512];
			snprintf(filepath, sizeof(filepath), "../cache/%s", song->id);

			printf("\nfilenamebackgroundtop====>%s\n",filepath);
			//char path[1024];
			//char *path = ytDownload(filepath, song->url);
			char *path = ytDownload(filepath, song->url);
			//../cache/id.mp3
			if (path) {
			printf("\nfilenamebackgroundintocache====>%s\n",path);
			int rc = CacheSong(cache,song,path, expires);
				printf("Saved: %s\n", path);
		//		free(path);
				_exit(rc != 0);
			 }
			// sqlite3 *cache = InitCache();
			// if (!cache){
			// 	perror("cache db init");
			// 	return 1;
			// }else{
			// 	printf("cache db works hjah\n");
			// }
				
		//sqlite3_close(app->cache);

		}
		_exit(0);
	} else if (pid > 0) {
		int status;
		waitpid(pid, &status, 0);
		// quick reap of the short-lived first child
		printf("cache started\n");
		return 0;
	} else {
		perror("fork");
		return -1;
	}
}

