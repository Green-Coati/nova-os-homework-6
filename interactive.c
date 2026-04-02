/*
 * interactive.c
 *
 * Sleeps for CLICK_INTERVAL_MS between "clicks" and reports:
 *   - elapsed time since click (ms, 3 decimals)
 *   - instantaneous jitter for THIS click (ms, 3 decimals)
 *
 * jitter_ms = actual_interval_ms - CLICK_INTERVAL_MS
 *
 * Usage:
 *   ./interactive <runtime_seconds>
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define CLICK_INTERVAL_MS 100.0


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("usage: %s <run-seconds>\n", argv[0]);
        return 1;
    }
    double totaltime = 0.0;
    double max = ((double) atoi(argv[1]) * 1000);
    struct timespec start;
    struct timespec end;
    while (totaltime < max) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        double startms = start.tv_sec * 1000.0 + start.tv_nsec / 1e6;
        usleep(CLICK_INTERVAL_MS * 1000);
        clock_gettime(CLOCK_MONOTONIC, &end);
        double endms = end.tv_sec * 1000.0 + end.tv_nsec / 1e6;
        double duration = (endms - startms);
        totaltime += duration;
        printf("[click] elapsed=%.3lf ms jitter=%.3lf ms\n", duration, duration - CLICK_INTERVAL_MS);
    }
    return 0;
}
