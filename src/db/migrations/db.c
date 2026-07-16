#include <stdio.h>
#include <sqlite3.h>

int main(void)
{
    sqlite3 *db;

    int rc = sqlite3_open("music.db", &db);

    if (rc != SQLITE_OK) {
        printf("Cannot open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    printf("Database opened\n");

    sqlite3_close(db);

    return 0;
}
