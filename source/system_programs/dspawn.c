#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Parent process exits immediately
        printf("Daemon spawned with PID: %d\n", pid);
        return 0;
    }

    // Child process becomes the daemon
    setsid(); // Detach from terminal

    // Optional: redirect stdin, stdout, stderr to /dev/null
    int null_fd = open("/dev/null", O_RDWR);
    dup2(null_fd, STDIN_FILENO);
    dup2(null_fd, STDOUT_FILENO);
    dup2(null_fd, STDERR_FILENO);

    // Simulate background work: write to a log file every few seconds
    FILE *log = fopen("dspawn_log.txt", "a");
    if (!log) exit(1);

    while (1) {
        fprintf(log, "dspawn daemon running in background...\n");
        fflush(log);
        sleep(5);
    }

    fclose(log);
    return 0;
}
