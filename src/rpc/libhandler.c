#include <cjson/cJSON.h>
#include "../db/playlists.h"
#include "../lib/queue.h"
#include "../yt/yt.h"
#include "../db/song.h"
#include "../db/queue.h"
#include "../player/mpv/mpv.h"
#include <stdio.h>
#include <string.h>


int libhandler(App *app, char *method, cJSON *params, int id) {
    int success = 0;
    cJSON *resp = cJSON_CreateObject();
    cJSON_AddNumberToObject(resp, "id", id);
    cJSON_AddStringToObject(resp, "method", method);

    int playlistid = 0;
    if (strcmp(method, "lib-createplaylist") == 0) {
        cJSON *title = cJSON_GetObjectItemCaseSensitive(params, "title");
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
        cJSON *playlistid_node = cJSON_GetObjectItemCaseSensitive(params, "playlistid");
        if (!playlistid_node) {
            playlistid_node = cJSON_GetObjectItemCaseSensitive(params, "playlistId");
        }
        cJSON *songName_node = cJSON_GetObjectItemCaseSensitive(params, "songName");
        cJSON *songid_node = cJSON_GetObjectItemCaseSensitive(params, "songid");

        if (playlistid_node) {
            if (songid_node && cJSON_IsString(songid_node)) {
                if (addsongtoplaylist(app->db, songid_node->valuestring, playlistid_node->valueint) == 0) {
                    success = 1;
                } else {
                    success = 0;
                }
            } else if (songName_node && cJSON_IsString(songName_node)) {
                Song *song = malloc(sizeof(Song));
                if (ytSearch(songName_node->valuestring, song) == 0) {
                    int r = CheckSong(app->db, song->id);
                    if (r == 0) {
                        AddSong(app->db, song);
                    }
                    if (r >= 0) {
                        if (addsongtoplaylist(app->db, song->id, playlistid_node->valueint) == 0) {
                            success = 1;
                        } else {
                            success = 0;
                        }
                    }
                }
                // Memory for song->id etc. should ideally be freed here, but matching existing code style
                free(song);
            }
        }
    } else if (strcmp(method, "lib-deletesongfromplaylist") == 0) {
        cJSON *playlistid = cJSON_GetObjectItemCaseSensitive(params, "playlistid");
        cJSON *songid = cJSON_GetObjectItemCaseSensitive(params, "songid");
        if (playlistid && songid && cJSON_IsString(songid) && deletesongfromplaylist(app->db, playlistid->valueint, songid->valuestring) == 0) {
            success = 1;
        }
    } else if (strcmp(method, "lib-getplaylist") == 0) {
        cJSON *playlistid = cJSON_GetObjectItemCaseSensitive(params, "playlistid");
        if (playlistid) {
            cJSON *playlist = getplaylist(app->db, playlistid->valueint);
            if (playlist) {
                success = 1;
                cJSON_AddItemToObject(resp, "playlist", playlist);
            }
        }
    } else if (strcmp(method, "lib-getallplaylists") == 0) {
        cJSON *playlists = getallplaylists(app->db);
        if (playlists) {
            success = 1;
            cJSON_AddItemToObject(resp, "playlists", playlists);
        }
    } else if (strcmp(method, "lib-getplaylistsongs") == 0) {
        cJSON *playlistid = cJSON_GetObjectItemCaseSensitive(params, "playlistid");
        if (playlistid) {
            cJSON *songs = getplaylistsongs(app->db, playlistid->valueint);
            if (songs) {
                success = 1;
                cJSON_AddItemToObject(resp, "songs", songs);
            }
        }
    }
    if (success) {
        cJSON_AddTrueToObject(resp, "success");
    } else {
        cJSON_AddFalseToObject(resp, "success");
    }
    cJSON_AddNumberToObject(resp, "id",playlistid);
    cmdresponse(resp, app->msgqueue,"playlist");
    return success ? 0 : -1;
}
//this is just a data struct that we keep on memory and we put into the db once the app closes or somthing
int queuehandler(App *app, char *method, cJSON *params, int id) {
    int success = 0;
    cJSON *resp = cJSON_CreateObject();
    cJSON_AddNumberToObject(resp, "id", id);
    cJSON_AddStringToObject(resp, "method", method);

    if (strcmp(method, "lib-addtoqueue") == 0) {
        cJSON *songid_node = cJSON_GetObjectItemCaseSensitive(params, "songid");
        if (songid_node && cJSON_IsString(songid_node)) {
            int qid = addtoqueue(app->songqueue, songid_node->valuestring);
            if (qid >= 0) {
                success = 1;
                cJSON_AddNumberToObject(resp, "queueid", qid);
            }
        } 
    } else if (strcmp(method, "lib-removefromqueue") == 0) {
        cJSON *songindex = cJSON_GetObjectItemCaseSensitive(params, "songindex");
        if (songindex) {
            if (removesongfromqueue(app->songqueue,songindex->valueint) == 0) {
                success = 1;
            }
        }
    } else if (strcmp(method, "lib-clearqueue") == 0) {
        if (clearqueue(app->songqueue) == 0) {
            success = 1;
        }
    } else if (strcmp(method, "lib-getqueuesongs") == 0) {

    cJSON *songlist = cJSON_CreateArray();

    for (int i = 0; i < app->songqueue->count; i++) {

        char *songid = app->songqueue->queue[i];

        Song song = {0};
		printf("methanada awula\n");
        GetSong(app->db, songid, &song);

		printf("methanada awula\n");
        cJSON *songobj = cJSON_CreateObject();

        cJSON_AddStringToObject(songobj, "songid", song.id ? song.id : songid);
        cJSON_AddStringToObject(songobj, "title", song.title ? song.title : "Unknown Title");
        cJSON_AddStringToObject(songobj, "artist", song.artist ? song.artist : "Unknown Artist");
        cJSON_AddBoolToObject(songobj, "isliked", song.isliked);
        cJSON_AddItemToArray(songlist, songobj);

        if (song.id) free(song.id);
        if (song.title) free(song.title);
        if (song.artist) free(song.artist);
        if (song.genre) free(song.genre);
    }

    cJSON_AddItemToObject(resp, "queuelist", songlist);
    } else if (strcmp(method, "lib-savequeue") == 0) {
        cJSON *queue = cJSON_GetObjectItemCaseSensitive(params, "queue");
        if (queue && cJSON_IsArray(queue)) {
            int count = cJSON_GetArraySize(queue);
            char **arr = malloc(count * sizeof(char*));
            for (int i = 0; i < count; i++) {
                cJSON *item = cJSON_GetArrayItem(queue, i);
                arr[i] = item->valuestring;
            }
            if (savequeue(app->db, arr, count) == 0) {
                success = 1;
            }
            free(arr);
        }
    } else if (strcmp(method, "lib-getsavedqueue") == 0) {
        char **out = NULL;
        int count = 0;
        cJSON *songlist = cJSON_CreateArray();
        
        if (getsavedqueue(app->db, &out, &count) == 0) {
            for (int i = 0; i < count; i++) {
                Song song = {0};
                if (GetSong(app->db, out[i], &song) == 0) {
                    cJSON *songobj = cJSON_CreateObject();
                    cJSON_AddStringToObject(songobj, "songid", song.id);
                    cJSON_AddStringToObject(songobj, "title", song.title);
                    cJSON_AddStringToObject(songobj, "artist", song.artist);
                    cJSON_AddBoolToObject(songobj, "isliked", song.isliked);
                    cJSON_AddItemToArray(songlist, songobj);
                    
                    if (song.id) free(song.id);
                    if (song.title) free(song.title);
                    if (song.artist) free(song.artist);
                    if (song.genre) free(song.genre);
                }
                free(out[i]);
            }
            free(out);
            success = 1;
        }
        cJSON_AddItemToObject(resp, "queuesongs", songlist);
    }

    if (success) {
        cJSON_AddTrueToObject(resp, "success");
    } else {
        cJSON_AddFalseToObject(resp, "success");
    }
    cmdresponse(resp, app->msgqueue,"queue");
    return success ? 0 : -1;
}

