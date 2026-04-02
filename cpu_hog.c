/*
 * cpu_hog.c
 *
 * CPU-bound infinite loop.
 *
 */

int main() {
    int i = 10;
    while (1) {
        if (i % 2 == 0) {
            i = i / 2;
        } else {
            i = 3 * i + 1;
        }
    }
    return 0;
}