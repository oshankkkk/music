#ifndef PLAYLISTS_H
#define PLAYLISTS_H

#include <sqlite3.h>
#include <cjson/cJSON.h>
#include "../models/app.h"

int createplaylist(sqlite3 *db, char *title, int *id);
int deleteplaylist(sqlite3 *db, int playlistid);
int deletesongfromplaylist(sqlite3 *db, int playlistid, int songid);
int renameplaylist(sqlite3 *db, int playlistid, char *newname);
int addsongtoplaylist(sqlite3 *db, int songid, int playlistid);
int libhandler(App *app, char *method, cJSON *params, int id);

#endif // PLAYLISTS_H
