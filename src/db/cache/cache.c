#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "../../models/song.h"
#include "../../models/cache.h"
#include "cache.h"

//static void check(int rc, sqlite3 *db) {
//    if (rc != SQLITE_OK && rc != SQLITE_DONE && rc != SQLITE_ROW)
//        fprintf(stderr, "slite error: %s\n", sqlite3_errmsg(db));
//}

sqlite3 * InitCache(void){
	sqlite3 *cache;	
	int rc=sqlite3_open("./db/migrations/cache.db",&cache);
	if( rc!=SQLITE_OK){
		return NULL;
	}
	return cache;
}

int CacheSong(sqlite3 *db,Song *song,char *filepath, time_t expiresAt ){
	sqlite3_stmt *stmt;
	const char *sql =
    "INSERT INTO audioCache (id, filepath, created_at, expires_at) "
    "VALUES (?, ?, ?, ?) "
    "ON CONFLICT(id) DO UPDATE SET "
    "filepath = excluded.filepath, "
    "created_at = excluded.created_at, "
    "expires_at = excluded.expires_at";	

	int rc= sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);
	if (rc!=SQLITE_OK){
    printf("sql error code: %d\n", sqlite3_errcode(db));
	return -1;
	}
char expiry[21];
snprintf(expiry, sizeof(expiry), "%lld", (long long)expiresAt);
//
char updated[21];
snprintf(updated, sizeof(updated), "%lld", (long long)song->playedTime);

sqlite3_bind_text(stmt, 1, song->id, -1, SQLITE_STATIC);
sqlite3_bind_text(stmt, 2, filepath, -1, SQLITE_STATIC);
sqlite3_bind_text(stmt, 3, updated, -1,SQLITE_STATIC);
sqlite3_bind_text(stmt, 4, expiry,-1, SQLITE_STATIC);

    int nrc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
	if (nrc != SQLITE_DONE) {
    fprintf(stderr, "SQLite Error: %s\n", sqlite3_errmsg(db));
    return -1;
}
return 0;
}

int GetCacheSong(sqlite3 *db,char *id,Cache *out){
char *str="SELECT filepath FROM audioCache WHERE id = ?";
sqlite3_stmt *stmt;
int rc=sqlite3_prepare_v2(db,str,1,&stmt,NULL);
	if (rc!=SQLITE_OK){
    printf("sql error code: %d\n", sqlite3_errcode(db));
	return 1;
	}
sqlite3_bind_text(stmt,1,id,-1, SQLITE_STATIC);

    int nrc = sqlite3_step(stmt);
	if (nrc==SQLITE_ROW){

		memset(out, 0, sizeof(Song));
		// zero all pointers so a failed strdup leaves NULL, not garbage
		out->filepath = strdup((const char *)sqlite3_column_text(stmt, 0));
		//out->expDate= strdup((const char *)sqlite3_column_text(stmt, 1));
		//out->creatDate= strdup((const char *)sqlite3_column_text(stmt, 2));

//		snprintf(out->filepath, sizeof(out->filepath), "%s", sqlite3_column_text(stmt, 0));
//		snprintf(out->expDate, sizeof(out->expDate), "%s", sqlite3_column_text(stmt, 1));
//		sqlite3_finalize(stmt);
//

	return 0;
	}
	sqlite3_finalize(stmt);
	if (nrc == SQLITE_DONE) {
        fprintf(stderr, "song not found\n");
        return 1;
    }

return 0;
}
int CheckCache(sqlite3 *db, char *id) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT EXISTS(SELECT 1 FROM audioCache WHERE id=?)";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        return -1;

    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);

    int exists = -1;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = sqlite3_column_int(stmt, 0); // 0 or 1
    }

    sqlite3_finalize(stmt);
    return exists;
}

void FreeSongList(Song *arr, int count) {
    for (int i = 0; i < count; i++) {
        free(arr[i].id);
        free(arr[i].title);
        free(arr[i].artist);
        free(arr[i].thumbnail);
        free(arr[i].uploaddate);
        free(arr[i].url);
        free(arr[i].genre);
    }
    free(arr);
}

