#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdbool.h>

int mpvRun(char *path,bool *isCached){
//printf("path  %s",path);
		pid_t pid = fork();
		if (pid == 0) {
			execlp(
					"mpv",
					"mpv",
					"--no-video",
					path,
					NULL
				  );

			perror("execlp");
			_exit(1);
		}
		else if (pid > 0) {
			printf("mpv started with PID %d\n", pid);
			int status;
			waitpid(pid, &status, 0);
		}
		else {
			perror("fork");
			return 1;		
		}
	if (*isCached){
			*isCached=false;
		}

	return 0;
}

