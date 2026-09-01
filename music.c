#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

static pid_t tui_pid = -1;
static pid_t backend_pid = -1;

void cleanup(void){

    printf("Shutting down...\n");

    if (backend_pid > 0) {
        printf("Stopping backend (PID %d)...\n", backend_pid);

        kill(backend_pid, SIGTERM);
        waitpid(backend_pid, NULL, 0);

        backend_pid = -1;
    }

    
    if (tui_pid > 0) {
        printf("Stopping TUI (PID %d)...\n", tui_pid);

        kill(tui_pid, SIGTERM);
        waitpid(tui_pid, NULL, 0);

        tui_pid = -1;
    }

    printf("Everything stopped.\n");
}

void handle_signal(int sig){

    (void)sig;

    cleanup();
    exit(0);
}

int main(void){
	FILE *logfp=fopen("log.txt", "w");

	if(logfp==NULL){
		perror("log file error");
		return 1;
	}
	
	int logfd=fileno(logfp);
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);


    backend_pid = fork();

    if (backend_pid < 0) {
        perror("fork backend");
        return 1;
	}
	char command[512];

	snprintf(
			command,
			sizeof(command),

			"gcc -g $(find src -name '*.c') "
			"-o build/music "
			"-Wall "
			"-Werror "
			"-lsqlite3 "
			"-lcjson "
			"&& ./build/music %d",

			logfd
		);
	if (backend_pid == 0) {

        execlp(

            "sh",
            "sh",
            "-c",
			command,
			NULL
        );

        perror("failed to start backend");
        _exit(1);
    }

	sleep(2);
    tui_pid = fork();

    if (tui_pid < 0) {
        perror("fork tui");

        cleanup();

        return 1;
    }

    if (tui_pid == 0) {

        if (chdir("tui") == -1) {
            perror("chdir tui");
            _exit(1);
        }

		execlp(
            "bun",
//			"SHOW_CONSOLE=true"
            "bun",
            "run",
            "dev",
            NULL
        );

		perror("failed to start TUI");
        _exit(1);
    }


    printf("Music player started.\n");
    printf("Backend PID: %d\n", backend_pid);
    printf("TUI PID:     %d\n", tui_pid);


	int status;

    waitpid(tui_pid, &status, 0);

    tui_pid = -1;

    printf("TUI exited.\n");


	cleanup();

    return 0;
}

