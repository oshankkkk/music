#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

sqlite3 * InitCache(void){
	sqlite3 *cache;	
	int rc=sqlite3_open("./db/migrations/cache.db",&cache);
	if( rc!=SQLITE_OK){
		return NULL;
	}
	return cache;
}

int CacheSong(sqlite3 *db,Song *song, char *filepath,char *expiresAt,char *createdAt){
	sqlite3_stmt *stmt;
	const char *sql=
		"INSERT INTO audioCache (id, filepath, created_at, expires_at)"
		"VALUES (?, ?, ?, ?)"
		"ON CONFLICT(id) DO UPDATE SET"
		"filepath = excluded.filepath"
		"created_at = excluded.created_at"
		"expires_at = excluded.expires_at";
	int rc= sqlite3_prepare_v2(db,sql,1,&stmt,NULL);
	if (rc!=SQLITE_OK){
    printf("sql error code: %d\n", sqlite3_errcode(db));
	return 1;
	}

sqlite3_bind_text(stmt, 1, song->id, -1, SQLITE_STATIC);
sqlite3_bind_text(stmt, 2, filepath, -1, SQLITE_STATIC);
sqlite3_bind_text(stmt, 3, createdAt, -1,SQLITE_STATIC);
sqlite3_bind_text(stmt, 4, expiresAt,-1, SQLITE_STATIC);

    int nrc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
	if (rc != SQLITE_DONE) {
    fprintf(stderr, "SQLite Error: %s\n", sqlite3_errmsg(db));
    return -1;
}
return 0;
}
int GetCacheSong(sqlite3 *db,char *id,Cache *out){
char *str="SELECT filepath, expires_at FROM audioCache WHERE id = ?";
sqlite3_stmt *stmt;
int rc=sqlite3_prepare_v2(db,str,1,&stmt,NULL);
	if (rc!=SQLITE_OK){
    printf("sql error code: %d\n", sqlite3_errcode(db));
	return 1;
	}
sqlite3_bind_text(stmt,1,id,-1, SQLITE_STATIC);

    int nrc = sqlite3_step(stmt);
	if (nrc==SQLITE_ROW){
    snprintf(out->filepath, sizeof(out->filepath), "%s", sqlite3_column_text(stmt, 0));
    snprintf(out->expDate, sizeof(out->expDate), "%s", sqlite3_column_text(stmt, 1));
	sqlite3_finalize(stmt);
	return 0;
	}
	sqlite3_finalize(stmt);
	if (nrc == SQLITE_DONE) {
        fprintf(stderr, "song not found\n");
        return 1;
    }

return 0;
}
