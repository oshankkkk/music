#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>

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
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    backend_pid = fork();

    if (backend_pid < 0) {
        perror("fork backend");
        return 1;
	}
	if (backend_pid == 0) {

        int nullfd = open("/dev/null", O_RDONLY);
        int logfd  = open("build/backend.log", O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (nullfd >= 0) {
            dup2(nullfd, STDIN_FILENO);
            close(nullfd);
        }
        if (logfd >= 0) {
            dup2(logfd, STDOUT_FILENO);
            dup2(logfd, STDERR_FILENO);
            close(logfd);
        }

        execlp(
            "sh",
            "sh",
            "-c",
			"gcc -g $(find src -name '*.c') "
			"-o build/music "
			"-Wall "
			"-Werror "
			"-lsqlite3 "
			"-lcjson "
			"&& ./build/music",
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
