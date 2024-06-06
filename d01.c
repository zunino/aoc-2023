#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define LINE_BUFFER_SIZE 128

size_t sum_calibration_values_1() {
    size_t sum = 0;

    FILE *fp = fopen("data/d01.txt", "r");
    char buffer[LINE_BUFFER_SIZE];
    while (fgets(buffer, LINE_BUFFER_SIZE, fp) != NULL) {
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

bool _look_ahead(const char *needle, const char *haystack) {
    for (int n = 0; n < strlen(needle); ++n) {
        if (needle[n] != haystack[n]) {
            return false;
        }
    }
    return true;
}

bool _look_behind(const char *needle, const char *haystack) {
    return _look_ahead(needle, haystack - strlen(needle));
}

int _check_spelled_number_fw(const char c,  const char* beg) {
    if (c == 'o' && _look_ahead("ne", beg)) return 1;
    if (c == 'e' && _look_ahead("ight", beg)) return 8;
    if (c == 'n' && _look_ahead("ine", beg)) return 9;
    if (c == 't') {
        if (_look_ahead("wo", beg)) return 2;
        if (_look_ahead("hree", beg)) return 3;
    }
    if (c == 'f') {
        if (_look_ahead("our", beg)) return 4;
        if (_look_ahead("ive", beg)) return 5;
    }
    if (c == 's') {
        if (_look_ahead("ix", beg)) return 6;
        if (_look_ahead("even", beg)) return 7;
    }
    return 0;
}

int _check_spelled_number_bw(const char c,  const char* end) {
    if (c == 'o' && _look_behind("tw", end)) return 2;
    if (c == 'r' && _look_behind("fou", end)) return 4;
    if (c == 'x' && _look_behind("si", end)) return 6;
    if (c == 'n' && _look_behind("seve", end)) return 7;
    if (c == 't' && _look_behind("eigh", end)) return 8;
    if (c == 'e') {
        if (_look_behind("on", end)) return 1;
        if (_look_behind("thre", end)) return 3;
        if (_look_behind("fiv", end)) return 5;
        if (_look_behind("nin", end)) return 9;
    }
    return 0;
}

size_t sum_calibration_values_2() {
    size_t sum = 0;

    FILE *fp = fopen("data/d01.txt", "r");
    char buffer[LINE_BUFFER_SIZE];
    while (fgets(buffer, LINE_BUFFER_SIZE, fp) != NULL) {
        const char *beg = &buffer[0];
        for (;;++beg) {
            char c = *beg;
            if (isdigit(c)) {
                sum += 10 * (c - '0');        
                break;
            }
            int value = _check_spelled_number_fw(c, beg + 1);
            if (value != 0) {
                sum += 10 * value;
                break;
            }
        }

        const char *end = &buffer[strlen(buffer) - 2];
        for (;;--end) {
            char c = *end;
            if (isdigit(c)) {
                sum += c - '0';        
                break;
            }
            int value = _check_spelled_number_bw(c, end);
            if (value != 0) {
                sum += value;
                break;
            }
        }
    }
    fclose(fp);

    return sum;
}

int main(void) {
    printf("Day 1 - part 1: %zu\n", sum_calibration_values_1());
    printf("Day 1 - part 2: %zu\n", sum_calibration_values_2());
}
