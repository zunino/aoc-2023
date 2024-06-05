#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include <string.h>

#define BUFFER_SIZE 128

size_t sum_calibration_values() {
    size_t sum = 0;

    FILE *fp = fopen("data/day-01.txt", "r");
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        const char *beg = &buffer[0];
        const char *end = &buffer[strlen(buffer) - 2];
        while (!isdigit(*beg)) ++beg;
        while (!isdigit(*end)) --end;
        unsigned short d1 = *beg - '0';
        unsigned short d2 = *end - '0';
        sum += d1 * 10 + d2;
    }
    fclose(fp);

    return sum;
}

int main(void) {
    printf("Day 1 - part 1: %zu\n", sum_calibration_values());
}
