#include "day6.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OBSTACLE ('#')
#define GUARD_VISITED ('X')

guard_t *find_guard(const char *line, const size_t line_len) {
    if (line == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < line_len; i++) {
        char ch = line[i];

        if (ch == '<') {
            guard_t *guard = calloc(1, sizeof(guard_t));
            guard->y_pos = i;
            guard->direction = LEFT;
            guard->has_left = false;
            return guard;
        }

        if (ch == '>') {
            guard_t *guard = calloc(1, sizeof(guard_t));
            guard->y_pos = i;
            guard->direction = RIGHT;
            guard->has_left = false;
            return guard;
        }

        if (ch == '^') {
            guard_t *guard = calloc(1, sizeof(guard_t));
            guard->y_pos = i;
            guard->direction = UP;
            guard->has_left = false;
            return guard;
        }

        if (ch == 'v') {
            guard_t *guard = calloc(1, sizeof(guard_t));
            guard->y_pos = i;
            guard->direction = DOWN;
            guard->has_left = false;
            return guard;
        }
    }

    return NULL;
}

world_t *load_world(const char *puzzle_input) {
    FILE *fp = fopen(puzzle_input, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to load file: %s\n", puzzle_input);
        return NULL;
    }

    guard_t *guard = NULL;
    uint16_t map_max_x = 0;
    uint16_t map_max_y = 0;

    size_t map_buffer_size = 10;
    char **map_buffer = calloc(map_buffer_size, sizeof(char *) * map_buffer_size);
    if (map_buffer == NULL) {
        return NULL;
    }

    char *line_buffer = calloc(3, sizeof(char));
    size_t line_buffer_size = 3;
    size_t line_len = 0;

    while ((line_len = getline(&line_buffer, &line_buffer_size, fp)) != -1) {
        if (line_buffer[line_len - 1] == '\n') {
            line_buffer[line_len - 1] = '\0';
            line_len--;
        }

        if (line_len > map_max_y) {
            map_max_y = line_len;
        }

        if (map_max_x + 1 > map_buffer_size) {
            char **new_buffer = realloc(map_buffer, sizeof(char *) * map_buffer_size * 2);
            if (new_buffer == NULL) {
                free(map_buffer);
                return NULL;
            }

            map_buffer = new_buffer;
            map_buffer_size *= 2;
        }

        if (guard == NULL) {
            guard = find_guard(line_buffer, line_len);
            if (guard != NULL) {
                guard->x_pos = map_max_x;
            }
        }

        map_buffer[map_max_x] = strdup(line_buffer);
        map_max_x++;
    }

    fclose(fp);
    free(line_buffer);

    // Check if we have invalid data
    if (guard == NULL) {
        for (size_t i = 0; i < map_max_x; i++) {
            free(map_buffer[i]);
        }
        free(map_buffer);
        return NULL;
    }

    map_t *map = calloc(1, sizeof(map_t));
    map->max_x = map_max_x;
    map->max_y = map_max_y;
    map->raw_map = map_buffer;

    world_t *world = calloc(1, sizeof(world_t));
    world->guard = guard;
    world->map = map;

    return world;
}

void destroy_world(world_t *world) {
    if (world == NULL) {
        return;
    }

    if (world->guard != NULL) {
        free(world->guard);
    }

    if (world->map != NULL) {
        for (size_t i = 0; i < world->map->max_x; i++) {
            free(world->map->raw_map[i]);
        }
        free(world->map->raw_map);
        free(world->map);
    }

    free(world);
}

void debug_dump_map(map_t *map) {
    for (uint16_t x = 0; x < map->max_x; x++) {
        for (uint16_t y = 0; y < map->max_y; y++) {
            printf("%c", map->raw_map[x][y]);
        }
        printf("\n");
    }

    printf("\n");
}

char get_direction_char(const guard_direction_t direction) {
    switch (direction) {
        case UP:
            return '^';
        case RIGHT:
            return '>';
        case DOWN:
            return 'v';
        case LEFT:
            return '<';
        default:
            return '?';
    }
}

void tick_world(const world_t *world) {
    // Mark current field as visited
    world->map->raw_map[world->guard->x_pos][world->guard->y_pos] = GUARD_VISITED;

    // Find the next field the guard would move to
    uint16_t next_guard_x = world->guard->x_pos;
    uint16_t next_guard_y = world->guard->y_pos;
    switch (world->guard->direction) {
        case UP:
            next_guard_x--;
            break;
        case DOWN:
            next_guard_x++;
            break;
        case LEFT:
            next_guard_y--;
            break;
        case RIGHT:
            next_guard_y++;
            break;
    }

    // Check if the guard must have left the world
    // Note that we're using unsigned types for position. So 0 - 1 would roll over.
    // Because of that, we just need to check if we're out of bounds in the "greater than" direction
    if (next_guard_x >= world->map->max_x || next_guard_y >= world->map->max_y) {
        world->guard->has_left = true;
        return;
    }

    // Check if the guard hit an obstacle and must turn
    if (world->map->raw_map[next_guard_x][next_guard_y] == OBSTACLE) {
        if (world->guard->direction == LEFT ) {
            world->guard->direction = UP;
        } else {
            world->guard->direction++; // Numeric enums are smart :)
        }
        return;
    }

    // No obstacle hit - Move the guard
    world->guard->x_pos = next_guard_x;
    world->guard->y_pos = next_guard_y;
    world->map->raw_map[next_guard_x][next_guard_y] = get_direction_char(world->guard->direction);
}

void solve_day_6(const char *puzzle_input) {
    world_t *world = load_world(puzzle_input);

    printf(
        "Loaded world! MAX_X: %d, MAX_Y: %d, GUARD_POS_X: %d, GUARD_POS_Y: %d, GUARD_DIR: %c\n",
        world->map->max_x,
        world->map->max_y,
        world->guard->x_pos,
        world->guard->y_pos,
        get_direction_char(world->guard->direction));

    while (world->guard->has_left == false) {
        tick_world(world);
    }

    int distinct_fields_visited = 0;
    for (uint16_t x = 0; x < world->map->max_x; x++) {
        for (uint16_t y = 0; y < world->map->max_y; y++) {
            if (world->map->raw_map[x][y] == GUARD_VISITED) {
                distinct_fields_visited++;
            }
        }
    }

    printf("Guard has left and has visited %d distinct fields!\n", distinct_fields_visited);
    destroy_world(world);
}
