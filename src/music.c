#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <cjson/cJSON.h>
#include <sqlite3.h>
#include "./models/song.c"
#include "./models/cache.c"
#include "./models/response.c"
#include "./db/cache/cache.c"
#include "./db/song.c"


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


int ytSearch(char  songName[2048], Respones *response){
	char cmd [1024];
	snprintf(cmd,sizeof(cmd),"yt-dlp --dump-json ytsearch1:%s",songName);
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


int main(void) {
	sqlite3 *db =InitDb();
	if (!db){
		perror("db init");
		return 1;
	}else{
		printf("db works\n");
	}
	
	sqlite3 *cache = InitCache();
	if (!cache){
		perror("cache db init");
		return 1;
	}else{
		printf("cache db works\n");
	}
	
	sqlite3_close(db);
	sqlite3_close(cache);
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

		Respones song;
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


