#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sqlite3.h>
sqlite3 * InitDb(void){
	sqlite3 *db;	
	int rc=sqlite3_open("./db/migrations/music.db",&db);
	if( rc!=SQLITE_OK){
		return NULL;
	}
	return db;

}

static void check(int rc, sqlite3 *db) {
    if (rc != SQLITE_OK && rc != SQLITE_DONE && rc != SQLITE_ROW)
        fprintf(stderr, "sqlite error: %s\n", sqlite3_errmsg(db));
}
int CheckSong(sqlite3 *db, const char *id, bool *exists) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT EXISTS(SELECT 1 FROM song WHERE id=?)";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        check(sqlite3_errcode(db), db);
        return -1;
    }
    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        *exists = sqlite3_column_int(stmt, 0) != 0;
        rc = 0;
    } else {
        check(rc, db);
        rc = -1;
    }
    sqlite3_finalize(stmt);
    return rc;
}

int AddSong(sqlite3 *db, const Song *s) {
    const char *sql =
        "INSERT INTO song (id, title, artist, duration, isliked, genre) "
        "VALUES (?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        check(sqlite3_errcode(db), db);
        return -1;
    }
    sqlite3_bind_text(stmt, 1, s->id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, s->title, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, s->artist, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, s->duration);
    sqlite3_bind_int(stmt, 5, s->isliked);
    sqlite3_bind_text(stmt, 6, s->genre, -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) {
        check(rc, db);
        return -1;
    }
    return 0;
}

int GetSong(sqlite3 *db, const char *id, Song *out) {
    const char *sql =
        "SELECT id, title, artist, duration, isliked, genre FROM song WHERE id=?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        check(sqlite3_errcode(db), db);
        return -1;
    }
    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        snprintf(out->id, sizeof(out->id), "%s", sqlite3_column_text(stmt, 0));
        snprintf(out->title, sizeof(out->title), "%s", sqlite3_column_text(stmt, 1));
        snprintf(out->artist, sizeof(out->artist), "%s", sqlite3_column_text(stmt, 2));
        out->duration = sqlite3_column_int(stmt, 3);
        out->isliked  = sqlite3_column_int(stmt, 4) != 0;
        snprintf(out->genre, sizeof(out->genre), "%s", sqlite3_column_text(stmt, 5));
        sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_finalize(stmt);
    if (rc == SQLITE_DONE) {
        fprintf(stderr, "song not found\n");
        return 1;

    }
    check(rc, db);
    return -1;
}

int remove_song(sqlite3 *db, const char *id) {
    bool exists;
    int rc = CheckSong(db, id, &exists);
    if (rc != 0) return rc;
    if (!exists) {
        fprintf(stderr, "song not found\n");
        return 1;
    }

    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM song WHERE id=?";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) { check(rc, db); return -1; }
    return 0;
}

static int updateSong(sqlite3 *db, const char *sql, bool has_bool, bool val, const char *id) {
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        check(sqlite3_errcode(db), db);
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
    if (rc != SQLITE_DONE) { check(rc, db); return -1; }

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
        check(sqlite3_errcode(db), db);
        return -1;
    }

    int cap = 16, n = 0;
    Song *arr = malloc(cap * sizeof(Song));

    int rc;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (n == cap) { cap *= 2; arr = realloc(arr, cap * sizeof(Song)); }
        Song *s = &arr[n++];
        snprintf(s->id, sizeof(s->id), "%s", sqlite3_column_text(stmt, 0));
        snprintf(s->title, sizeof(s->title), "%s", sqlite3_column_text(stmt, 1));
        snprintf(s->artist, sizeof(s->artist), "%s", sqlite3_column_text(stmt, 2));
        s->duration = sqlite3_column_int(stmt, 3);
        s->isliked  = sqlite3_column_int(stmt, 4) != 0;
        snprintf(s->genre, sizeof(s->genre), "%s", sqlite3_column_text(stmt, 5));
    }
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) { check(rc, db); free(arr); return -1; }
    *out = arr;
    *count = n;
    return 0;
}

