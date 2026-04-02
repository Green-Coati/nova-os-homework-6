/*
 * sched_test.c
 *
 * Run:
 *   ./sched_test <num-hogs> <runtime-seconds>
 *
 * Required behavior:
 *   1) fork+exec CPU hogs first
 *   2) fork+exec interactive last (passing runtime-seconds)
 *   3) waitpid() for interactive process
 *   4) kill hogs
 *   5) reap remaining children
 */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_hogs> <runtime_seconds>\n", argv[0]);
        return 1;
    }

    int numHogs = atoi(argv[1]);

    pid_t *hogPids = malloc(numHogs * sizeof(int));

    for (int i = 0; i < numHogs; i++) { // release the hogs
        pid_t pid = fork();
        if (pid == 0) { // in child
            char *cwd = malloc(256 * sizeof(char));
            getcwd(cwd, 256);
            char *path = malloc((strlen(cwd) + 9) * sizeof(char));
            strcpy(path, cwd);
            strcat(path, "/cpu_hog");
            char **args = malloc(sizeof(char*));
            args[0] = malloc((strlen(path) + 1) * sizeof(char));
            strcpy(args[0], path);
            if (execv(path, args) == -1) {
                printf("error: permission denied\n");
            }
            free(args[0]);
            free(args);
            free(path);
            free(cwd);
        } else {
            hogPids[i] = pid;
        }
    }

    pid_t interactPid = fork();
    if (interactPid == 0) { // in child
        char *cwd = malloc(256 * sizeof(char));
        getcwd(cwd, 256);
        char *path = malloc((strlen(cwd) + 13) * sizeof(char));
        strcpy(path, cwd);
        strcat(path, "/interactive");
        char **args = malloc(2 * sizeof(char*));
        args[0] = malloc((strlen(path) + 1) * sizeof(char));
        args[1] = malloc((strlen(argv[2]) + 1) * sizeof(char));
        strcpy(args[0], path);
        strcpy(args[1], argv[2]);
        if (execv(path, args) == -1) {
            printf("error: permission denied\n");
        }
        free(args[0]);
        free(args);
        free(path);
        free(cwd);
    }

    wait(NULL);

    for (int i = 0; i < numHogs; i++) {
        kill(hogPids[i], SIGKILL);
    }

    for (int i = 0; i < numHogs; i++) {
        wait(NULL);
    }

    return 0;
}