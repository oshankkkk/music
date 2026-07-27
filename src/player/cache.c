#include <stdio.h>
#include <sys/wait.h>
#include <strings.h>
#include <unistd.h>
#include <stdbool.h>
#include <cjson/cJSON.h>
#include <sqlite3.h>
#include "../db/cache/cache.c"
#include "../yt/yt.c"
//#include "./server/server.c"

int backgroundCaching(Song *song) {
	time_t expires = song->playedTime+ (7 * 24 * 60 * 60);
	pid_t pid = fork();
	if (pid == 0) {
		if (fork() == 0) {
			char filepath[512];
			snprintf(filepath, sizeof(filepath), "../cache/%s", song->id);
			char *path = ytDownload(filepath, song->url);
			if (path) {
				printf("Saved: %s\n", path);
				free(path);
			}
			sqlite3 *cache = InitCache();
			if (!cache){
				perror("cache db init");
				return 1;
			}else{
				printf("cache db works\n");
			}
			int rc = CacheSong(cache, song,path, expires);
			sqlite3_close(cache);

			_exit(rc != 0);
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

