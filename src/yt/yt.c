#include <cjson/cJSON.h>
#include "../models/response.h"
#include "yt.h"
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

char *ytDownload(char *filepath, char *url)
{
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

int ytSearch(char  songName[2048], Respones *response){
	char cmd [1024];
//	snprintf(cmd,sizeof(cmd),"yt-dlp --dump-json ytsearch1:%s",songName);
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

	memset(response, 0, sizeof(*response));

	if (cJSON_IsString(id))       snprintf(response->id, sizeof(response->id), "%s", id->valuestring);
	if (cJSON_IsString(title))    snprintf(response->title, sizeof(response->title), "%s", title->valuestring);
	if (cJSON_IsString(artist))   snprintf(response->artist, sizeof(response->artist), "%s", artist->valuestring);
	if (cJSON_IsNumber(duration)) response->duration = duration->valuedouble;
	if (cJSON_IsString(thumb))    snprintf(response->thumbnail, sizeof(response->thumbnail), "%s", thumb->valuestring);
	if (cJSON_IsNumber(views))    response->view_count = (long)views->valuedouble;
	if (cJSON_IsString(upload))   snprintf(response->upload_date, sizeof(response->upload_date), "%s", upload->valuestring);
	if (cJSON_IsString(url))      snprintf(response->url, sizeof(response->url), "%s", url->valuestring);

	cJSON_Delete(root);
	return 0;
}

