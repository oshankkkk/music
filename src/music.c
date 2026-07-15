#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void) {
	char songName [100];
	printf("Music player\n");
	printf("Enter song Name\n");
	char *err =fgets(songName,sizeof(songName),stdin);

	if (err==NULL){
		perror("popen");
		return 1;
	}		

	songName[strcspn(songName, "\n")] = '\0';

    char cmd[256];

    snprintf(
        cmd,
        sizeof(cmd),
        "yt-dlp -f bestaudio -g --no-playlist ytsearch1:%s",
        songName
    );

	//FILE *p = popen("yt-dlp -f bestaudio -g --no-playlist 'sugarcrash' ","r");
	FILE *p = popen(cmd,"r");

	if (p==NULL){
		perror("popen");
		return 1;
	}		

	char url[4096];	
	char *ptr=fgets(url,sizeof(url),p);

	if (ptr==NULL){
		perror("popen");
		return 1;
	};	

	pid_t pid = fork();

    if (pid == 0) {
        // child process
        execlp(
            "mpv",
            "mpv",
            "--no-video",
            url,
            NULL
        );

        perror("execlp");
    }
    else if (pid > 0) {
        // parent process
        printf("mpv started with PID %d\n", pid);
    }
    else {
        perror("fork");
    }

//	for (int i=0;url[i];i++){
//	printf("%c",url[i]);
//	}

return 0;
}

