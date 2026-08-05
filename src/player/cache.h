#ifndef PLAYER_CACHE_H
#define PLAYER_CACHE_H

#include <sqlite3.h>
#include "../models/song.h"

int backgroundCaching(Song *song, sqlite3 *cache);

#endif
