#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <cjson/cJSON.h>
 
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

typedef struct {
    char id[64];
    char title[256];
    char artist[128];
    double duration;
    char thumbnail[512];
    long view_count;
    char upload_date[16];
    char url[512];
} Song;


int ytSearch(char  songName[2048], Song *song){
	char cmd [1024];
	snprintf(cmd,sizeof(cmd),"yt-dlp --dump-json ytsearch1:%s",songName);
	FILE *ptr=popen(cmd,"r");
	if (!ptr){
		perror("search ytdlp error");
	}
	char *response=readAll(ptr);

    int close_status = pclose(ptr);
 
	if (!response) {
        fprintf(stderr, "failed to read yt-dlp output\n");
        return 1;
    }
    if (close_status != 0 || response[0] == '\0') {
        fprintf(stderr, "yt-dlp returned no results\n");
        free(response);
        return 1;
    }
 
    cJSON *root = cJSON_Parse(response);
    free(response);
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
    if (cJSON_IsString(id))       snprintf(song->id, sizeof(song->id), "%s", id->valuestring);
    if (cJSON_IsString(title))    snprintf(song->title, sizeof(song->title), "%s", title->valuestring);
    if (cJSON_IsString(artist))   snprintf(song->artist, sizeof(song->artist), "%s", artist->valuestring);
    if (cJSON_IsNumber(duration)) song->duration = duration->valuedouble;
    if (cJSON_IsString(thumb))    snprintf(song->thumbnail, sizeof(song->thumbnail), "%s", thumb->valuestring);
    if (cJSON_IsNumber(views))    song->view_count = (long)views->valuedouble;
    if (cJSON_IsString(upload))   snprintf(song->upload_date, sizeof(song->upload_date), "%s", upload->valuestring);
    if (cJSON_IsString(url))      snprintf(song->url, sizeof(song->url), "%s", url->valuestring);
 
    cJSON_Delete(root);
return 0;
}

int main(void) {
	char songName [2048];
	printf("Music player\n");
	while(true){

		printf("Enter song Name or (exit to quit)>>> ");

		char *err =fgets(songName,sizeof(songName),stdin);
		songName[strcspn(songName, "\n")] = '\0';

		if (strcmp(songName,"exit")==0){
			break;
		}

		if (err==NULL){
			perror("popen");
			return 1;
		}		

        Song song;
        if (ytSearch(songName, &song) != 0) {
            fprintf(stderr, "search failed, try again\n");
            continue;
        }
        if (song.url[0] == '\0') {
            fprintf(stderr, "no url found for that result\n");
            continue;
        }
 
        printf("Playing: %s - %s\n", song.title, song.artist);
 
		pid_t pid = fork();
		if (pid == 0) {
			execlp(
					"mpv",
					"mpv",
					"--no-video",
					song.url,
					NULL
				  );

			perror("execlp");
		}
		else if (pid > 0) {
			printf("mpv started with PID %d\n", pid);
		}
		else {
			perror("fork");
		}

	}

	return 0;
}


