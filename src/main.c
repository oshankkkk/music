#include <stdio.h>
#include <sys/wait.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <cjson/cJSON.h>
#include <sqlite3.h>
#include "./models/song.h"
#include "./models/cache.h"
#include "./models/response.h"
#include "./db/song.c"
#include "./db/cache/cache.c"
#include "./yt/yt.c"
#include "./server/server.c"

int backgroundCaching(Song *song) {
	time_t expires = song->playedTime+ (7 * 24 * 60 * 60);
	pid_t pid = fork();
	if (pid == 0) {
		if (fork() == 0) {
			char filepath[512];
			printf("caching");
			snprintf(filepath, sizeof(filepath), "../cache/%s", song->id);
			char *path = ytDownload(filepath, song->url);
			if (path) {
				printf("Saved: %s\n", path);
				free(path);
			}
			// grandchild does the real work, gets reparented, no zombie risk
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
		_exit(0); // first child exits immediately, parent reaps it fast
	} else if (pid > 0) {
		int status;
		waitpid(pid, &status, 0); // quick reap of the short-lived first child
		printf("cache started\n");
		return 0;
	} else {
		perror("fork");
		return -1;
	}
}
int main(void){
serverInit();
return 0;
}
int main33(void) {
	bool isCached = false;
	sqlite3 *db =InitDb();
	if (!db){
		perror("db init");
		return 1;
	}else{
		printf("db works\n");
	}
	sqlite3 *cache = InitCache();
			if (!cache){
				perror("cache db init");
				return 1;
			}else{
				printf("cache db works\n");
			}
	char songName [2048];
	printf("Music player\n");
	while(true){

		printf("Enter song Name or (exit to quit)>>> ");

		char *err =fgets(songName,sizeof(songName),stdin);
		songName[strcspn(songName, "\n")] = '\0';

		if (strcmp(songName,"exit")==0){
			break;
		}

		if (err==NULL){
			perror("popen");
			return 1;
		}		

		Respones response;
		if (ytSearch(songName, &response) != 0) {
			fprintf(stderr, "search failed, try again\n");
			continue;
		}
		if (response.url[0] == '\0') {
			fprintf(stderr, "no url found for that result\n");
			continue;
		}
time_t updated = time(NULL);
Song song = {
    .id = response.id,
    .title = response.title,
    .artist = response.artist,
    .duration = response.duration,
    .url = response.url,
    .playedTime= updated,
};
if (isCached){
isCached=false;
}
		int check=CheckSong(db, song.id);
		printf("this happend");
		Cache cachesong;
		char *path;
		if (check==-1){
			AddSong(db, &song);
			path=song.url;
			backgroundCaching(&song);
			// start background caching
		}else{
			int cacheCheck=CheckCache(cache,song.id);
				printf("meka thama cache line eke %d \n",cacheCheck);
			if (cacheCheck==1){
				isCached=true;
				GetCacheSong(cache,song.id,&cachesong);	
				path=cachesong.filepath;	
			}else if (cacheCheck==-1){
				printf("sql error");
				break;
			}else{
				path=song.url;
				// start background caching
				backgroundCaching(&song);
			}
		}
		//printf("Playing: %s - %s and here is the path=> %s \n", song.title, song.artist,path);
		char *songpath;
		if( isCached){
			songpath=path;
		}else{
			songpath=song.url;	
		}
		printf("path  %s",path);
		pid_t pid = fork();
		if (pid == 0) {
			execlp(
					"mpv",
					"mpv",
					"--no-video",
					songpath,
					NULL
				  );

			perror("execlp");
			_exit(1);
		}
		else if (pid > 0) {
			printf("mpv started with PID %d\n", pid);
			int status;
			waitpid(pid, &status, 0);
		}
		else {
			perror("fork");
		}

	}
	sqlite3_close(db);
	sqlite3_close(cache);
	return 0;

}


