#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SCHEMATIC_ROWS 140
#define SCHEMATIC_COLS 140

#define GEAR_SYMBOL '*'

unsigned char schematic[SCHEMATIC_ROWS][SCHEMATIC_COLS];

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

ValueLoc extract_value(int y, int x) {
    char value_str[8] = {0};
    int n_digits = 0;
    for (int c = 0; isdigit(schematic[y][x + c]); ++c) {
        value_str[c] = schematic[y][x + c];
        ++n_digits;
    }
    return (ValueLoc) {
        .value = atoi(value_str),
        .x_end = x + n_digits
    };
}

bool is_symbol(unsigned char c) {
    return c != '.' && !isdigit(c);
}

typedef struct {
    int y_min;
    int y_max;
    int x_min;
    int x_max;
} Bounds;

Bounds find_bounds(int y, int x_beg, int x_end) {
    return (Bounds) {
        .y_min = y > 0 ? y - 1 : y,
        .y_max = y < SCHEMATIC_ROWS - 1 ? y + 1 : y,
        .x_min = x_beg > 0 ? x_beg - 1 : x_beg,
        .x_max = x_end < SCHEMATIC_COLS - 1 ? x_end + 1 : x_end,
    };
}

bool symbol_adjacency_test(int y, int x_beg, int x_end) {
    Bounds b = find_bounds(y, x_beg, x_end);

    // above
    if (b.y_min < y) {
        for (int x = b.x_min; x <= b.x_max; ++x) {
            if (is_symbol(schematic[b.y_min][x])) return true;
        }
    }

    // left
    if (b.x_min < x_beg && is_symbol(schematic[y][b.x_min])) return true;

    // right
    if (b.x_max > x_end && is_symbol(schematic[y][b.x_max])) return true;

    // below
    if (b.y_max > y) {
        for (int x = b.x_min; x <= b.x_max; ++x) {
            if (is_symbol(schematic[b.y_max][x])) return true;
        }
    }

    return false;
}

size_t part_1_sum_of_part_numbers() {
    size_t sum = 0;
    for (int i = 0; i < SCHEMATIC_ROWS; ++i) {
        for (int j = 0; j < SCHEMATIC_COLS; ++j) {
            if (isdigit(schematic[i][j])) {
                ValueLoc vl = extract_value(i, j);
                if (symbol_adjacency_test(i, j, vl.x_end - 1)) {
                    sum += vl.value;
                }
                j = vl.x_end - 1;
            }
        }
    }
    return sum;
}

// ==[ PART 2 ]=====

bool is_gear(int y, int x) {
    return schematic[y][x] == GEAR_SYMBOL;
}

int find_number_x_beg(int y, int x) {
    int x_off = x;
    while (x_off - 1 >= 0 && isdigit(schematic[y][x_off - 1])) {
        --x_off;
    }
    return x_off;
}

ValueLoc extract_value_rev(int y, int x) {
    int x_beg = find_number_x_beg(y, x);
    return extract_value(y, x_beg);
}

typedef struct {
    int numbers[6];
    int n_adjacencies;
} AdjacentNumbers;

AdjacentNumbers find_adjacent_numbers(int gy, int gx) {
    AdjacentNumbers adj = {0};
    Bounds b = find_bounds(gy, gx, gx);

    // above
    if (b.y_min < gy) {
        int x_beg = b.x_min;
        if (isdigit(schematic[b.y_min][x_beg])) {
            x_beg = find_number_x_beg(b.y_min, x_beg);
        }
        for (int x = x_beg; x <= b.x_max; ++x) {
            if (isdigit(schematic[b.y_min][x])) {
                ValueLoc vl = extract_value(b.y_min, x);
                adj.numbers[adj.n_adjacencies++] = vl.value;
                x = vl.x_end;
            }
        }
    }
   
    // left
    if (b.x_min < gx && isdigit(schematic[gy][b.x_min])) {
        ValueLoc vl = extract_value_rev(gy, b.x_min);
        adj.numbers[adj.n_adjacencies++] = vl.value;
    }
   
    // right
    if (b.x_max > gx && isdigit(schematic[gy][b.x_max])) {
        ValueLoc vl = extract_value(gy, b.x_max);
        adj.numbers[adj.n_adjacencies++] = vl.value;
    }
   
    // below
    if (b.y_max > gy) {
        int x_beg = b.x_min;
        if (isdigit(schematic[b.y_max][x_beg])) {
            x_beg = find_number_x_beg(b.y_max, x_beg);
        }
        for (int x = x_beg; x <= b.x_max; ++x) {
            if (isdigit(schematic[b.y_max][x])) {
                ValueLoc vl = extract_value(b.y_max, x);
                adj.numbers[adj.n_adjacencies++] = vl.value;
                x = vl.x_end;
            }
        }
    }

    return adj;
}

size_t part_2_sum_of_gear_ratios() {
    size_t sum = 0;
    for (int i = 0; i < SCHEMATIC_ROWS; ++i) {
        for (int j = 0; j < SCHEMATIC_COLS; ++j) {
            if (is_gear(i, j)) {
                AdjacentNumbers adj = find_adjacent_numbers(i, j);
                if (adj.n_adjacencies == 2) {
                    sum += adj.numbers[0] * adj.numbers[1];
                }
            }
        }
    }
    return sum;
}

int main(void) {
    load_schematic();
    printf("Day 3 - part 1: %zu\n", part_1_sum_of_part_numbers());
    printf("Day 3 - part 2: %zu\n", part_2_sum_of_gear_ratios());
}
