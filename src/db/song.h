#ifndef DB_SONG_H
#define DB_SONG_H

#include <sqlite3.h>
#include "../models/song.h"

sqlite3 * InitDb(void);
int CheckSong(sqlite3 *db, const char *id);
int AddSong(sqlite3 *db, const Song *s);
int GetSong(sqlite3 *db, const char *id, Song *s);
int remove_song(sqlite3 *db, const char *id);
int play_song(sqlite3 *db, const char *id);
int like_song(sqlite3 *db, const char *id);
int unlike_song(sqlite3 *db, const char *id);
int GetSongList(sqlite3 *db, Song **out, int *count);

#endif // DB_SONG_H
