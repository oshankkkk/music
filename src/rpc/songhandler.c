#include <cjson/cJSON.h>
#include "../player/mpv/mpv.h"
#include "../player/player.h"
#include "../db/song.h"
#include "./response.h"
#include <string.h>
#include <stdio.h>


int songhandler(App *app, char *method, cJSON *params, int id) {
    int success = 0;
    cJSON *resp = cJSON_CreateObject();
    cJSON_AddNumberToObject(resp, "id", id);
    cJSON_AddStringToObject(resp, "method", method);

	//printf("method------->%s",method);
	if (strcmp(method, "song-playSong") == 0){

		cJSON *songName = cJSON_GetObjectItemCaseSensitive(params, "songName");
	//	printf("songname-->%s",songName->valuestring);
		if (songName == NULL || !cJSON_IsString(songName)) {
			rpcerror(-32602, "Invalid params: requires string songName", id, app->msgqueue);
			return -1;
		}
		success=playSong(app,songName->valuestring);
		if (success!=0){
			perror("rpc start song");
		}

		if (app->currentsong != NULL) {
			cJSON *song=cJSON_CreateObject();	
			cJSON_AddStringToObject(song, "songid", app->currentsong->id ? app->currentsong->id : "");
			cJSON_AddStringToObject(song, "title", app->currentsong->title ? app->currentsong->title : "");
			cJSON_AddStringToObject(song, "artist", app->currentsong->artist ? app->currentsong->artist : "");
			cJSON_AddNumberToObject(song, "duration", app->currentsong->duration);
			cJSON_AddBoolToObject(song, "isliked", app->currentsong->isliked);
			cJSON_AddNumberToObject(song, "personalplaycount", app->currentsong->personalplaycount);

            cJSON_AddItemToObject(resp, "songlist",song);
			success=1;
		}
	}else if (strcmp(method, "song-getallsongs") == 0) {
        Song *songs = NULL;
        int count = 0;
        if (GetSongList(app->db, &songs, &count) == 0) {
            cJSON *songlist = cJSON_CreateArray();
            for (int i = 0; i < count; i++) {
                cJSON *song = cJSON_CreateObject();
                cJSON_AddStringToObject(song, "id", songs[i].id ? songs[i].id : "");
                cJSON_AddStringToObject(song, "title", songs[i].title ? songs[i].title : "");
                cJSON_AddStringToObject(song, "artist", songs[i].artist ? songs[i].artist : "");
                cJSON_AddNumberToObject(song, "duration", songs[i].duration);
                cJSON_AddBoolToObject(song, "isliked", songs[i].isliked);
                cJSON_AddStringToObject(song, "genre", songs[i].genre ? songs[i].genre : "");
                cJSON_AddItemToArray(songlist, song);

                free(songs[i].id);
                free(songs[i].title);
                free(songs[i].artist);
                free(songs[i].genre);
            }
            free(songs);
            cJSON_AddItemToObject(resp, "songlist", songlist);
            success = 1;
        }
    } else if (strcmp(method, "song-likesong") == 0) {
        cJSON *song_id = cJSON_GetObjectItem(params, "id");
        if (cJSON_IsString(song_id) && song_id->valuestring) {
            success = (like_song(app->db, song_id->valuestring) == 0);
        }
    } else if (strcmp(method, "song-unlikesong") == 0) {
        cJSON *song_id = cJSON_GetObjectItem(params, "id");
        if (cJSON_IsString(song_id) && song_id->valuestring) {
            success = (unlike_song(app->db, song_id->valuestring) == 0);
        }
    } else {
        success = 0;
    }
    if (success) {
        cJSON_AddTrueToObject(resp, "success");
    } else {
        cJSON_AddFalseToObject(resp, "success");
    }
    cmdresponse(resp, app->msgqueue, "song");
    return success ? 0 : -1;
}
