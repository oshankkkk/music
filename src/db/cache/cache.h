#ifndef DB_CACHE_H
#define DB_CACHE_H

#include <sqlite3.h>
#include "../../models/song.h"
#include "../../models/cache.h"

sqlite3 * InitCache(void);
int CacheSong(sqlite3 *db, Song *song, char *filepath);
int GetCacheSong(sqlite3 *db, char *id, Cache *out);
int CheckCache(sqlite3 *db, char *id);
void FreeSongList(Song *arr, int count);

#endif // DB_CACHE_H
