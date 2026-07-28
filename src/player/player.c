#include <stdio.h>
#include <sys/wait.h>
#include <strings.h>
#include "../mpv/mpv.c"
#include <unistd.h>
#include <stdbool.h>
#include <cjson/cJSON.h>
#include <sqlite3.h>
#include "../models/song.h"
#include "../models/cache.h"
#include "../db/song.c"
#include "./cache.c"
#include "../models/app.h"


int getSong(App *app,char *songName){
	//take rpc connection with songname from dispacher
	if (ytSearch(songName,app) != 0) {
		fprintf(stderr, "search failed, try again\n");
		return 1;
		//RPCerror
	}

	printf("getting song");
	if (app->currentsong->url[0] == '\0') {
		fprintf(stderr, "no url found for that result\n");
		return 1;
		//RPCerror
	}
	time_t updated = time(NULL);
	app->currentsong->playedTime=updated;
	return 0;
	//cacheing and mpv sne data back to the rpc
	//connect ipc with rpc
}

int getAudioPath(char **path,App *app){
	int check=CheckSong(app->db, app->currentsong->id);
	printf("this happend");
	Cache cachesong;
	if (check==-1){
		AddSong(app->db, app->currentsong);
		*path=app->currentsong->url;
		backgroundCaching(app->currentsong);
		// start background caching
	}else{
		int cacheCheck=CheckCache(app->cache,app->currentsong->id);
		printf("meka thama cache line eke %d \n",cacheCheck);
		if (cacheCheck==1){
			app->currentsong->isCached=true;
			GetCacheSong(app->cache,app->currentsong->id,&cachesong);	
			*path=cachesong.filepath;	
			//songpath=cachesong.filepath;	
		}else if (cacheCheck==-1){
			perror("getaudio");
			return 1;
		}else{
			*path=app->currentsong->url;
			// start background caching
			backgroundCaching(app->currentsong);
		}
	}
	return 0;
} 

int playSong(App *app,char *songName) {

	//RPC call to search, search gets you the song name
	// when user press play it calls the init play and then gets the audiopath and runsmpv

	int err=0;	
	char *path=NULL;

	err = getSong(app, songName);

    if (err != 0) {
        perror("initPlay");
			return err;
    }


	err = getAudioPath(&path, app);
    if (err != 0) {
        perror("audio");
			return err;
    }

    mpvRun(path);
	return 0;
}


