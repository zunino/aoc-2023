#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUFFER_SIZE 256

#define RED_CUBES 12
#define GREEN_CUBES 13
#define BLUE_CUBES 14

typedef struct {
    int red;
    int green;
    int blue;
} CubeSet;

typedef struct {
    int id;
    CubeSet sets[10];
    size_t n_sets;
} Game;

CubeSet parse_cube_set(char *set) {
    char *token = strtok(set, ",");
    CubeSet cube_set = {0};
    while (token) {
        int count;
        char color[8];
        sscanf(token, "%d %s", &count, color);
        if (strcmp(color, "red") == 0) {
            cube_set.red = count;
        } else if (strcmp(color, "green") == 0) {
            cube_set.green = count;
        } else if (strcmp(color, "blue") == 0) {
            cube_set.blue = count;
        }
        token = strtok(NULL, ",");
    }
    return cube_set;
}

Game parse_game(char* game_str) {
    char *token = strtok(game_str, ":");
    char *label = token;
    char *sets = strtok(NULL, ":");

    int id;
    char label_str_part[5];
    sscanf(label, "%s %d", label_str_part, &id);
    Game game = {
        .id = id,
        .n_sets = 0
    };
   
    char *end_str;
    token = strtok_r(sets, ";", &end_str);
    while (token) {
        CubeSet cube_set = parse_cube_set(token);
        game.sets[game.n_sets++] = cube_set;
        token = strtok_r(NULL, ";", &end_str);
    }
    return game;
}

bool is_game_possible(const Game *game) {
    for (int i = 0; i < game->n_sets; ++i) {
        CubeSet set = game->sets[i];
        if (set.red > RED_CUBES) return false;
        if (set.green > GREEN_CUBES) return false;
        if (set.blue > BLUE_CUBES) return false;
    }
    return true;
}

int id_sum_of_possible_games() {
    FILE *fp = fopen("data/d02.txt", "r");

    int id_sum = 0;
    char buffer[LINE_BUFFER_SIZE];
    while (fgets(buffer, LINE_BUFFER_SIZE, fp) != NULL) {
        Game game = parse_game(buffer);
        if (is_game_possible(&game)) {
            id_sum += game.id;
        }
    }

    fclose(fp);
    return id_sum;
}

int main(void) {
    printf("Day 2 - part 1: %d\n", id_sum_of_possible_games());
}
