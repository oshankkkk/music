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
//#include "../models/ytresponse.h"
//#include "../db/cache/cache.c"
//#include "./server/server.c"
//#include "../yt/yt.c"


bool *isCached = false;
typedef struct{
	sqlite3 *db;
	sqlite3 *cache;
	Song *currentsong;
}App;


int initPlay(App app,char *songName){
	//take rpc connection with songname from dispacher
	if (ytSearch(songName, app.currentsong) != 0) {
		fprintf(stderr, "search failed, try again\n");
		return 1;
		//RPCerror
	}
	if (app.currentsong->url[0] == '\0') {
		fprintf(stderr, "no url found for that result\n");
		return 1;
		//RPCerror
	}
	time_t updated = time(NULL);
	app.currentsong->playedTime=updated;
	return 0;
	//cacheing and mpv sne data back to the rpc
	//connect ipc with rpc
}

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
	return 0;
}

int getAudioPath(char *path,App app){
	int check=CheckSong(app.db, app.currentsong->id);
	printf("this happend");
	Cache cachesong;
	if (check==-1){
		AddSong(app.db, app.currentsong);
		path=app.currentsong->url;
		backgroundCaching(app.currentsong);
		// start background caching
	}else{
		int cacheCheck=CheckCache(app.cache,app.currentsong->id);
		printf("meka thama cache line eke %d \n",cacheCheck);
		if (cacheCheck==1){
			*isCached=true;
			GetCacheSong(app.cache,app.currentsong->id,&cachesong);	
	//}
			path=cachesong.filepath;	
			//songpath=cachesong.filepath;	
		}else if (cacheCheck==-1){
			perror("getaudio");
			return 1;
		}else{
			path=app.currentsong->url;
			// start background caching
			backgroundCaching(app.currentsong);
		}
	}
	return 0;
} 

int player(void) {
	//starts when the app starts
	//
	char *songName;
	App app;
	char *path;
	int err=0;
	err = startup(&app);
    if (err != 0) {
        perror("startup");
        goto cleanup;
    }

	//RPC call to search, search gets you the song name
	// when user press play it calls the init play and then gets the audiopath and runsmpv
	
	err = initPlay(app, songName);
    if (err != 0) {
        perror("initPlay");
        goto cleanup;
    }

	err = getAudioPath(path, app);
    if (err != 0) {
        perror("audio");
        goto cleanup;
    }

    mpvRun(path, isCached);

cleanup:
    if (app.db)    sqlite3_close(app.db);
    if (app.cache) sqlite3_close(app.cache);
    return err;

}    





