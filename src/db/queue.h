#ifndef DB_QUEUE_H
#define DB_QUEUE_H

#include <sqlite3.h>

int getsavedqueue(sqlite3 *db, char ***out, int *count);
int savequeue(sqlite3 *db, char **arr, int count);

#endif // DB_QUEUE_H
