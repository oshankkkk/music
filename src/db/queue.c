#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void handleError(int rc, sqlite3 *db) {
    fprintf(stderr, "SQLite Error: %s\n", sqlite3_errmsg(db));
}

int getsavedqueue(sqlite3 *db, char ***out, int *count) {
    const char *sql = "SELECT song_id FROM queue";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        handleError(sqlite3_errcode(db), db);
        return -1;
    }

    int cap = 16;
    int n = 0;

    char **arr = malloc(cap * sizeof(*arr));

    if (!arr) {
        sqlite3_finalize(stmt);
        return -1;
    }

    int rc;

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (n == cap) {
            cap *= 2;

            char **tmp = realloc(arr, cap * sizeof(*arr));
            if (!tmp) {
                // Not freeing individual strings here for simplicity in error handling
                free(arr);
                sqlite3_finalize(stmt);
                return -1;
            }

            arr = tmp;
        }

        const char *txt = (const char *)sqlite3_column_text(stmt, 0);
        arr[n] = txt ? strdup(txt) : strdup("");
        n++;
    }

    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        handleError(rc, db);
        for(int i=0; i<n; i++) free(arr[i]);
        free(arr);
        return -1;
    }

    *out = arr;
    *count = n;

    return 0;
}

int savequeue(sqlite3 *db, char **arr, int count) {
    char *errmsg = NULL;
    if (sqlite3_exec(db, "DELETE FROM queue", NULL, NULL, &errmsg) != SQLITE_OK) {
        fprintf(stderr, "SQLite Error: %s\n", errmsg);
        sqlite3_free(errmsg);
        // continue anyway or return -1? Let's return -1
        return -1;
    }

    const char *sql =
        "INSERT INTO queue (song_id) "
        "VALUES (?)";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        handleError(sqlite3_errcode(db), db);
        return -1;
    }

    for (int i = 0; i < count; i++) {
        sqlite3_bind_text(stmt, 1, arr[i], -1, SQLITE_TRANSIENT);

        int rc = sqlite3_step(stmt);

        if (rc != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            handleError(rc, db);
            return -1;
        }

        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
    }

    sqlite3_finalize(stmt);
    return 0;
}
