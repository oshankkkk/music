#include <sqlite3.h>
#include <cjson/cJSON.h>
#include <stdio.h>
#include <string.h>
#include "../models/app.h"
#include "../player/mpv/mpv.h"
#include <stddef.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

static void handleError(int rc, sqlite3 *db) {
    if (rc != SQLITE_OK && rc != SQLITE_DONE && rc != SQLITE_ROW)
        fprintf(stderr, "sqlite erroe33r: %s\n", sqlite3_errmsg(db));
}

int createplaylist(sqlite3 *db, char *title, int *id) {
    const char *sql = "INSERT INTO playlist (title) VALUES (?)";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        handleError(sqlite3_errcode(db), db);
        return -1;
    }
    sqlite3_bind_text(stmt, 1, title, -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) {
        handleError(rc, db);
        return -1;
    }
    if (id) {
        *id = (int)sqlite3_last_insert_rowid(db);
    }
    return 0;
}

int deleteplaylist(sqlite3 *db, int playlistid) {
    const char *sql = "DELETE FROM playlist WHERE id=?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        handleError(sqlite3_errcode(db), db);
        return -1;
    }
    sqlite3_bind_int(stmt, 1, playlistid);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) {
        handleError(rc, db);
        return -1;
    }
    return 0;
}

int deletesongfromplaylist(sqlite3 *db, int playlistid, const char *songid) {
    const char *sql = "DELETE FROM collection WHERE song_id=? AND playlist_id=?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        handleError(sqlite3_errcode(db), db);
        return -1;
    }


    sqlite3_bind_text(stmt, 1, songid, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, playlistid);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) {
        handleError(rc, db);
        return -1;
    }
    return 0;
}

int renameplaylist(sqlite3 *db, int playlistid, char *newname) {
    const char *sql = "UPDATE playlist SET title = ? WHERE id = ?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        handleError(sqlite3_errcode(db), db);
        return -1;
    }
    sqlite3_bind_text(stmt, 1, newname, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, playlistid);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) {
        handleError(rc, db);
        return -1;
    }
    return 0;
}

int addsongtoplaylist(sqlite3 *db, const char *songid, int playlistid) {
    const char *sql = "INSERT INTO collection(playlist_id,song_id) VALUES (?, ?)";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        handleError(sqlite3_errcode(db), db);
        return -1;
    }
    sqlite3_bind_int(stmt, 1, playlistid);
    sqlite3_bind_text(stmt, 2, songid, -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) {
        handleError(rc, db);
        return -1;
    }
    return 0;
}

cJSON *getplaylist(sqlite3 *db, int playlistid) {
    const char *sql = "SELECT id, title, createdAt, songCount FROM playlist WHERE id = ?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        handleError(sqlite3_errcode(db), db);
        return NULL;
    }
    sqlite3_bind_int(stmt, 1, playlistid);

    cJSON *playlist = NULL;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        playlist = cJSON_CreateObject();
        cJSON_AddNumberToObject(playlist, "id", sqlite3_column_int(stmt, 0));
        cJSON_AddStringToObject(playlist, "title", (const char *)sqlite3_column_text(stmt, 1));
        cJSON_AddStringToObject(playlist, "createdAt", (const char *)sqlite3_column_text(stmt, 2));
        cJSON_AddNumberToObject(playlist, "songCount", sqlite3_column_int(stmt, 3));
    }
    sqlite3_finalize(stmt);
    return playlist;
}

cJSON *getallplaylists(sqlite3 *db) {
    const char *sql = "SELECT id, title,createdAt, songCount FROM playlist";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        handleError(sqlite3_errcode(db), db);
        return NULL;
    }

    cJSON *playlists = cJSON_CreateArray();
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cJSON *playlist = cJSON_CreateObject();
        cJSON_AddNumberToObject(playlist, "id", sqlite3_column_int(stmt, 0));
        cJSON_AddStringToObject(playlist, "title", (const char *)sqlite3_column_text(stmt, 1));
        cJSON_AddStringToObject(playlist, "createdAt", (const char *)sqlite3_column_text(stmt, 2));
        cJSON_AddNumberToObject(playlist, "songCount", sqlite3_column_int(stmt, 3));
        cJSON_AddItemToArray(playlists, playlist);
    }
    sqlite3_finalize(stmt);
    return playlists;
}

cJSON *getplaylistsongs(sqlite3 *db, int playlistid) {
    const char *sql = "SELECT s.id, s.title, s.artist, s.duration, s.isLiked "
                      "FROM song s "
                      "JOIN collection c ON s.id = c.song_id "
                      "WHERE c.playlist_id = ?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        handleError(sqlite3_errcode(db), db);
        return NULL;
    }
    sqlite3_bind_int(stmt, 1, playlistid);

    cJSON *songs = cJSON_CreateArray();
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cJSON *song = cJSON_CreateObject();
        const char *id_text = (const char *)sqlite3_column_text(stmt, 0);
        if (id_text) cJSON_AddStringToObject(song, "id", id_text);
        
        const char *title_text = (const char *)sqlite3_column_text(stmt, 1);
        if (title_text) cJSON_AddStringToObject(song, "title", title_text);
        
        const char *artist_text = (const char *)sqlite3_column_text(stmt, 2);
        if (artist_text) cJSON_AddStringToObject(song, "artist", artist_text);
        
        cJSON_AddNumberToObject(song, "duration", sqlite3_column_int(stmt, 3));
        cJSON_AddBoolToObject(song, "isLiked", sqlite3_column_int(stmt, 4));
        cJSON_AddItemToArray(songs, song);
    }
    sqlite3_finalize(stmt);
    return songs;
}


