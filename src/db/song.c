#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <time.h>
#include "../models/song.h"
#include "../models/app.h"
#include <cjson/cJSON.h>
#include "../player/mpv/mpv.h"
#include "../player/player.h"
#include "../rpc/response.h"
//#include "song.h"

sqlite3 * InitDb(void){
	sqlite3 *db;	
	int rc=sqlite3_open("./src/db/db/music.db",&db);
	if( rc!=SQLITE_OK){
		return NULL;
	}
	return db;
}

static void handleError(int rc, sqlite3 *db) {
    if (rc != SQLITE_OK && rc != SQLITE_DONE && rc != SQLITE_ROW)
        fprintf(stderr, "sqlite erroe33r: %s\n", sqlite3_errmsg(db));
}

int CheckSong(sqlite3 *db, const char *id) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT EXISTS(SELECT 1 FROM song WHERE id=?)";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        handleError(sqlite3_errcode(db), db);
        return -1;
    }
    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        return sqlite3_column_int(stmt, 0) != 0;
    } else {
		handleError(rc, db);
        rc = -1;
    }
    sqlite3_finalize(stmt);
    return rc;
}

int AddSong(sqlite3 *db, const Song *s) {
	time_t now=time(NULL);
    const char *sql =
        "INSERT INTO song (id, title, artist, duration, isliked, genre, lastPlayed) "
        "VALUES (?, ?, ?, ?, ?, ?, ?,)";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        handleError(sqlite3_errcode(db), db);
        return -1;
    }
    sqlite3_bind_text(stmt, 1, s->id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, s->title, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, s->artist, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, s->duration);
    sqlite3_bind_int(stmt, 5, s->isliked);
   	 
    sqlite3_bind_text(stmt, 6, s->genre, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 7,now);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) {
        handleError(rc, db);
        return -1;
    }
    return 0;
}

int GetSong(sqlite3 *db, const char *id, Song *s) {
    const char *sql =
        "SELECT id, title, artist, duration, isliked, genre FROM song WHERE id=?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        handleError(sqlite3_errcode(db), db);
        return -1;
    }
    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {

      memset(s, 0, sizeof(Song)); // zero all pointers so a failed strdup leaves NULL, not garbage
		s->id     = strdup((const char *)sqlite3_column_text(stmt, 0));
        s->title  = strdup((const char *)sqlite3_column_text(stmt, 1));
        s->artist = strdup((const char *)sqlite3_column_text(stmt, 2));
        s->duration = sqlite3_column_int(stmt, 3);
        s->isliked  = sqlite3_column_int(stmt, 4) != 0;
        s->genre  = strdup((const char *)sqlite3_column_text(stmt, 5));

        return 0;
    }
    sqlite3_finalize(stmt);
    if (rc == SQLITE_DONE) {
        fprintf(stderr, "song not found\n");
        return 1;

    }
    handleError(rc, db);
    return -1;
}

int remove_song(sqlite3 *db, const char *id) {
    int rc = CheckSong(db, id);
    if (rc != -1){
	return rc;
	}else{
        fprintf(stderr, "song not found\n");
        return 1;
	}
    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM song WHERE id=?";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) { handleError(rc, db); return -1; }
    return 0;
}

static int updateSong(sqlite3 *db, const char *sql, bool has_bool, bool val, const char *id) {
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        handleError(sqlite3_errcode(db), db);
        return -1;
    }
    if (has_bool) {
        sqlite3_bind_int(stmt, 1, val);
        sqlite3_bind_text(stmt, 2, id, -1, SQLITE_STATIC);
    } else {
        sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);
    }
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) { handleError(rc, db); return -1; }

    if (sqlite3_changes(db) == 0) { // equivalent of RowsAffected() == 0
        fprintf(stderr, "song not found\n");
        return 1;
    }
    return 0;
}

int play_song(sqlite3 *db, const char *id) {
    return updateSong(db, "UPDATE song SET playcount = playcount + 1 WHERE id=?", false, false, id);
}

int like_song(sqlite3 *db, const char *id) {
    return updateSong(db, "UPDATE song SET isliked=? WHERE id=?", true, true, id);
}
int unlike_song(sqlite3 *db, const char *id) {
    return updateSong(db, "UPDATE song SET isliked=? WHERE id=?", true, false, id);
}

int GetSongList(sqlite3 *db, Song **out, int *count) {
    const char *sql = "SELECT id, title, artist, duration, isliked, genre FROM song";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        handleError(sqlite3_errcode(db), db);
        return -1;
    }

    int cap = 16, n = 0;
    Song *arr = malloc(cap * sizeof(Song));

    int rc;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (n == cap) { cap *= 2; arr = realloc(arr, cap * sizeof(Song)); }
        Song *s = &arr[n++];

       memset(s, 0, sizeof(Song)); // zero all pointers so a failed strdup leaves NULL, not garbage
		s->id     = strdup((const char *)sqlite3_column_text(stmt, 0));
        s->title  = strdup((const char *)sqlite3_column_text(stmt, 1));
        s->artist = strdup((const char *)sqlite3_column_text(stmt, 2));
        s->duration = sqlite3_column_int(stmt, 3);
        s->isliked  = sqlite3_column_int(stmt, 4) != 0;
        s->genre  = strdup((const char *)sqlite3_column_text(stmt, 5));

    }
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) { handleError(rc, db); free(arr); return -1; }
    *out = arr;
    *count = n;
    return 0;
}

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
			cJSON_AddStringToObject(song, "songid", app->currentsong->id);
			cJSON_AddStringToObject(song, "title", app->currentsong->title);
			cJSON_AddStringToObject(song, "artist", app->currentsong->artist);
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
