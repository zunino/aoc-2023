#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SCHEMATIC_ROWS 140
#define SCHEMATIC_COLS 140

char schematic[SCHEMATIC_ROWS][SCHEMATIC_COLS];

void load_schematic() {
    FILE *fp = fopen("data/d03.txt", "r");
    int c;
    for (int i = 0; i < SCHEMATIC_ROWS; ++i) {
        for (int j = 0; (c = fgetc(fp)) != EOF && j < SCHEMATIC_COLS; ++j) {
            schematic[i][j] = c;
        }
    }
    fclose(fp);
}

typedef struct {
    int value;
    int x_end;
} ValueLoc;

ValueLoc extract_value(int i, int j) {
    char value_str[8] = {0};
    int n_digits = 0;
    for (int c = 0; isdigit(schematic[i][j + c]); ++c) {
        value_str[c] = schematic[i][j + c];
        ++n_digits;
    }
    return (ValueLoc) {
        .value = atoi(value_str),
        .x_end = j + n_digits
    };
}

bool is_symbol(unsigned char c) {
    return c != '.' && !isdigit(c);
}

bool symbol_adjacency_test(int y, int x_beg, int x_end) {
    int y_min = y > 0 ? y - 1 : y;
    int y_max = y < SCHEMATIC_ROWS - 1 ? y + 1 : y;
    int x_min = x_beg > 0 ? x_beg - 1 : x_beg;
    int x_max = x_end < SCHEMATIC_COLS - 1 ? x_end + 1 : x_end;

    // above
    if (y_min < y) {
        for (int x = x_min; x <= x_max; ++x) {
            if (is_symbol(schematic[y_min][x])) return true;
        }
    }
    // left
    if (x_min < x_beg) {
        if (is_symbol(schematic[y][x_min])) return true;
    }
    // right
    if (x_max > x_end) {
        if (is_symbol(schematic[y][x_max])) return true;
    }
    // bottom
    if (y_max > y) {
        for (int x = x_min; x <= x_max; ++x) {
            if (is_symbol(schematic[y_max][x])) return true;
        }
    }

    return false;
}

size_t part_1_sum_of_part_numbers() {
    size_t sum = 0;
    for (int i = 0; i < SCHEMATIC_ROWS; ++i) {
        for (int j = 0; j < SCHEMATIC_COLS; ++j) {
            if (isdigit(schematic[i][j])) {
                ValueLoc value_loc = extract_value(i, j);
                if (symbol_adjacency_test(i, j, value_loc.x_end - 1)) {
                    sum += value_loc.value;
                }
                j = value_loc.x_end - 1;
            }
        }
    }
    return sum;
}

int main(void) {
    load_schematic();
    printf("Day 3 - part 1: %zu\n", part_1_sum_of_part_numbers());
}
