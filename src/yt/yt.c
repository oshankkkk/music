#include <cjson/cJSON.h>
//#include "../models/ytresponse.h"
#include "yt.h"
#include "../models/song.h"
#include "../models/app.h"
//#include <cstddef>
//#include <cstring>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>


char * readAll(FILE *fp){
	size_t cap = 65536, len = 0;
	char *buf = malloc(cap);
	if (!buf) return NULL;

	size_t n;
	while ((n = fread(buf + len, 1, cap - len, fp)) > 0) {
		len += n;
		if (len == cap) {
			cap *= 2;
			char *tmp = realloc(buf, cap);
			if (!tmp) { free(buf); return NULL; }
			buf = tmp;
		}
	}
	buf[len] = '\0';
	return buf;
}

char *ytDownload(char *filepath, char *url){
    char cmd[2024];

    snprintf(cmd, sizeof(cmd),
        "yt-dlp -f bestaudio -x --audio-format mp3 -o \"%s.%%(ext)s\" \"%s\"",
        filepath, url);

    printf("Downloading...\n");

    int result = system(cmd);

    if (result != 0) {
        fprintf(stderr, "yt-dlp failed\n");
        return NULL;
    }

    char *output = malloc(strlen(filepath) + 5);

    if (!output)
        return NULL;

    sprintf(output, "%s.mp3", filepath);

    return output;
}

int ytSearch(char *songName, App *app){
	//Song *song;
	Song *song = malloc(sizeof(Song));
	song->isCached=false;	

	char cmd [1024];
	snprintf(cmd, sizeof(cmd),
    "yt-dlp --dump-json --no-playlist --no-warnings ytsearch1:%s 2>/dev/null",
    songName);
	FILE *ptr=popen(cmd,"r");
	if (!ptr){
		perror("search ytdlp error");
	}
	char *data=readAll(ptr);

	int close_status = pclose(ptr);

	if (!data) {
		fprintf(stderr, "failed to read yt-dlp output\n");
		return 1;
	}

	if (close_status != 0 ||data[0] == '\0') {
		fprintf(stderr, "yt-dlp returned no results\n");
		free(data);
		return 1;
	}

	cJSON *root = cJSON_Parse(data);
	free(data);
	if (!root) {
		fprintf(stderr, "JSON parse error near: %s\n", cJSON_GetErrorPtr());
		return 1;
	}

	cJSON *id       = cJSON_GetObjectItemCaseSensitive(root, "id");
	cJSON *title    = cJSON_GetObjectItemCaseSensitive(root, "title");

	cJSON *artist   = cJSON_GetObjectItemCaseSensitive(root, "uploader");
	cJSON *duration = cJSON_GetObjectItemCaseSensitive(root, "duration");
	cJSON *thumb    = cJSON_GetObjectItemCaseSensitive(root, "thumbnail");
	cJSON *views    = cJSON_GetObjectItemCaseSensitive(root, "view_count");
	cJSON *upload   = cJSON_GetObjectItemCaseSensitive(root, "upload_date");
	cJSON *url      = cJSON_GetObjectItemCaseSensitive(root, "webpage_url");

	memset(song, 0, sizeof(*song));
if (cJSON_IsString(id))
    song->id = strdup(id->valuestring);

if (cJSON_IsString(title))
    song->title = strdup(title->valuestring);

if (cJSON_IsString(artist))
    song->artist = strdup(artist->valuestring);

if (cJSON_IsNumber(duration))
    song->duration = duration->valuedouble;

if (cJSON_IsString(thumb))
    song->thumbnail = strdup(thumb->valuestring);

if (cJSON_IsNumber(views))
    song->viewCount = (i64)views->valuedouble;

if (cJSON_IsString(upload))
    song->uploadDate = strdup(upload->valuestring);

if (cJSON_IsString(url))
    song->url = strdup(url->valuestring);
	cJSON_Delete(root);
	app->currentsong=song;
	return 0;

}

void freeSong(Song *song)
{
    free(song->id);
    free(song->title);
    free(song->artist);
    free(song->thumbnail);
    free(song->uploadDate);
    free(song->url);
    free(song->genre);
}
