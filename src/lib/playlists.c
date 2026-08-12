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

int deletesongfromplaylist(sqlite3 *db, int playlistid, int songid) {
    const char *sql = "DELETE FROM collection WHERE songid=? AND playlistid=?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        handleError(sqlite3_errcode(db), db);
        return -1;
    }

    sqlite3_bind_int(stmt, 1, songid);
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

int addsongtoplaylist(sqlite3 *db, int songid, int playlistid) {
    const char *sql = "INSERT INTO collection(playlistid,songid) VALUES (?, ?)";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        handleError(sqlite3_errcode(db), db);
        return -1;
    }
    sqlite3_bind_int(stmt, 1, playlistid);
    sqlite3_bind_int(stmt, 2, songid);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) {
        handleError(rc, db);
        return -1;
    }
    return 0;
}

int libhandler(App *app, char *method, cJSON *params, int id) {
    int success = 0;
    cJSON *resp = cJSON_CreateObject();
    cJSON_AddNumberToObject(resp, "id", id);
    cJSON_AddStringToObject(resp, "method", method);

    if (strcmp(method, "lib-createplaylist") == 0) {
        cJSON *title = cJSON_GetObjectItemCaseSensitive(params, "title");
        int playlistid = 0;
        if (title && createplaylist(app->db, title->valuestring, &playlistid) == 0) {
            success = 1;
            cJSON_AddNumberToObject(resp, "playlistid", playlistid);
        }
    } else if (strcmp(method, "lib-removeplaylist") == 0) {
        cJSON *playlistid = cJSON_GetObjectItemCaseSensitive(params, "playlistid");
        if (playlistid && deleteplaylist(app->db, playlistid->valueint) == 0) {
            success = 1;
        }
    } else if (strcmp(method, "lib-renameplaylist") == 0) {
        cJSON *playlistid = cJSON_GetObjectItemCaseSensitive(params, "playlistid");
        cJSON *newname = cJSON_GetObjectItemCaseSensitive(params, "newname");
        if (playlistid && newname && renameplaylist(app->db, playlistid->valueint, newname->valuestring) == 0) {
            success = 1;
        }
    } else if (strcmp(method, "lib-addsongtoplaylist") == 0) {
        cJSON *playlistid = cJSON_GetObjectItemCaseSensitive(params, "playlistid");
        cJSON *songid = cJSON_GetObjectItemCaseSensitive(params, "songid");
        if (playlistid && songid && addsongtoplaylist(app->db, songid->valueint, playlistid->valueint) == 0) {
            success = 1;
        }
    } else if (strcmp(method, "lib-deletesongfromplaylist") == 0) {
        cJSON *playlistid = cJSON_GetObjectItemCaseSensitive(params, "playlistid");
        cJSON *songid = cJSON_GetObjectItemCaseSensitive(params, "songid");
        if (playlistid && songid && deletesongfromplaylist(app->db, playlistid->valueint, songid->valueint) == 0) {
            success = 1;
        }
    }

    if (success) {
        cJSON_AddTrueToObject(resp, "success");
    } else {
        cJSON_AddFalseToObject(resp, "success");
    }
    cmdresponse(resp, app->msgqueue,"playlist");
    return success ? 0 : -1;
}
