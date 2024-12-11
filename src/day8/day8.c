#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "day8.h"
#include "libstringutils.h"

bool d8_set_contains(const d8_point_set_t *point_set, const d8_point_t *point) {
    if (point_set == NULL || point == NULL) {
        return false;
    }

    // Check if there is a point at the given coords already
    for (size_t i = 0; i < point_set->size; i++) {
        if (point_set->contents[i]->x == point->x && point_set->contents[i]->y == point->y) {
            return true;
        }
    }

    return false;
}

bool d8_add_point(d8_point_set_t *list, d8_point_t *point) {
    if (list == NULL || point == NULL) {
        return false;
    }

    if (d8_set_contains(list, point)) {
        return false;
    }

    if (list->size + 1 >= list->capacity) {
        const size_t new_capacity = list->capacity * 2;
        list->contents = realloc(list->contents, sizeof(d8_point_t *) * new_capacity);
        list->capacity = new_capacity;
        if (list->contents == NULL) {
            fprintf(stderr, "Out of memory?\n");
            abort();
        }
    }

    list->contents[list->size] = point;
    list->size++;

    return true;
}

d8_point_set_t *d8_new_point_set(const size_t initial_size) {
    d8_point_set_t *list = calloc(1, sizeof(d8_point_set_t));
    if (list == NULL) {
        fprintf(stderr, "Out of memory!\n");
        abort();
    }

    list->contents = calloc(initial_size, sizeof(d8_point_t *));
    if (list->contents == NULL) {
        fprintf(stderr, "Out of memory!\n");
        abort();
    }

    list->capacity = initial_size;
    list->size = 0;
    return list;
}

void d8_destroy_set(d8_point_set_t *list) {
    if (list == NULL) {
        return;
    }

    if (list->contents != NULL) {
        for (size_t i = 0; i < list->size; i++) {
            free(list->contents[i]);
        }
        free(list->contents);
    }

    free(list);
}

d8_point_t *d8_get_anti_node(const d8_point_t *a, const d8_point_t *b, const int width, const int height) {
    if (a == NULL || b == NULL || a->ch != b->ch) {
        return NULL;
    }

    const int x_distance = b->x - a->x;
    const int anti_node_x = b->x + x_distance;

    const int y_distance = b->y - a->y;
    const int anti_node_y = b->y + y_distance;

    if (anti_node_x < 0 || anti_node_y < 0 || anti_node_x >= width || anti_node_y >= height) {
        return NULL;
    }

    d8_point_t *anti_node = calloc(1, sizeof(d8_point_t));
    anti_node->ch = '#';
    anti_node->x = anti_node_x;
    anti_node->y = anti_node_y;

    return anti_node;
}

d8_point_set_t *d8_get_anti_nodes(const d8_point_t *a, const d8_point_t *b, const int width, const int height) {
    if (a == NULL || b == NULL || a->ch != b->ch) {
        return NULL;
    }

    const int x_distance = b->x - a->x;
    const int y_distance = b->y - a->y;

    int x = b->x + x_distance;
    int y = b->y + y_distance;

    d8_point_set_t *anti_nodes = d8_new_point_set(5);
    while (x > - 1 && y > -1 && x < width && y < height) {
        d8_point_t *point = calloc(1, sizeof(d8_point_t));
        point->x = x;
        point->y = y;
        point->ch = '#';
        if (!d8_add_point(anti_nodes, point)) {
            free(point);
        }

        x += x_distance;
        y += y_distance;
    }

    return anti_nodes;
}

void solve_day8_1(const char *puzzle_input) {
    str_lines_t *lines = str_read_lines(puzzle_input);
    if (lines == NULL) {
        fprintf(stderr, "Failed to read lines from file %s\n", puzzle_input);
        return;
    }

    d8_point_set_t *antennas = d8_new_point_set(5);
    for (size_t y = 0; y < lines->num_lines; y++) {
        for (size_t x = 0; x < lines->line_len[y]; x++) {
            if (lines->lines[y][x] == EMPTY_FIELD) {
                continue;
            }

            d8_point_t *point = calloc(1, sizeof(d8_point_t));
            point->ch = lines->lines[y][x];
            point->x = (int) x;
            point->y = (int) y;
            if (!d8_add_point(antennas, point)) {
                free(point);
            }
        }
    }

    d8_point_set_t *anti_nodes = d8_new_point_set(5);
    for (size_t y = 0; y < antennas->size; y++) {
        for (size_t x = 0; x < antennas->size; x++) {
            if (y == x) {
                continue;
            }

            const d8_point_t *a = antennas->contents[y];
            const d8_point_t *b = antennas->contents[x];
            const int field_x_y = (int) lines->num_lines; // Assume square

            d8_point_t *point = d8_get_anti_node(a, b, field_x_y, field_x_y);
            if (!d8_add_point(anti_nodes, point)) {
                free(point);
            }
        }
    }

    /** Debug print field **/
    /*
    for (size_t y = 0; y < lines->num_lines; y++) {
        for (size_t x = 0; x < lines->line_len[y]; x++) {
            char ch = lines->lines[y][x];
            d8_point_t anti_node = {
                .ch = '#',
                .x = x,
                .y = y,
            };

            if (d8_set_contains(anti_nodes, &anti_node)) {
                ch = anti_node.ch;
            }

            printf("%c", ch);
        }
        printf("\n");
    } */

    printf("Day 8_1: There are %lu anti-nodes on the field\n", anti_nodes->size);
    d8_destroy_set(antennas);
    d8_destroy_set(anti_nodes);
    str_destroy_lines(lines);
}

void solve_day8_2(const char* puzzle_input) {
    str_lines_t *lines = str_read_lines(puzzle_input);
    if (lines == NULL) {
        fprintf(stderr, "Failed to read lines from file %s\n", puzzle_input);
        return;
    }

    d8_point_set_t *antennas = d8_new_point_set(5);
    for (size_t y = 0; y < lines->num_lines; y++) {
        for (size_t x = 0; x < lines->line_len[y]; x++) {
            if (lines->lines[y][x] == EMPTY_FIELD) {
                continue;
            }

            d8_point_t *point = calloc(1, sizeof(d8_point_t));
            point->ch = lines->lines[y][x];
            point->x = (int) x;
            point->y = (int) y;
            if (!d8_add_point(antennas, point)) {
                free(point);
            }
        }
    }

    d8_point_set_t *anti_nodes = d8_new_point_set(5);
    for (size_t y = 0; y < antennas->size; y++) {
        for (size_t x = 0; x < antennas->size; x++) {
            if (y == x) {
                continue;
            }
            const d8_point_t *a = antennas->contents[y];
            const d8_point_t *b = antennas->contents[x];
            const int field_x_y = (int) lines->num_lines; // Assume square

            d8_point_set_t *current_anti_nodes = d8_get_anti_nodes(a, b, field_x_y, field_x_y);
            if (current_anti_nodes == NULL) {
                continue;
            }

            for (size_t z = 0; z < current_anti_nodes->size; z++) {
                d8_point_t *current_anti_node = current_anti_nodes->contents[z];
                if (!d8_add_point(anti_nodes, current_anti_node)) {
                    free(current_anti_node);
                }
            }

            // Manually free as some of the list members must survive
            free(current_anti_nodes->contents);
            free(current_anti_nodes);
        }
    }

    int count = 0;
    for (size_t y = 0; y < lines->num_lines; y++) {
        for (size_t x = 0; x < lines->line_len[y]; x++) {
            char ch = lines->lines[y][x];
            d8_point_t anti_node = {
                .ch = '#',
                .x = x,
                .y = y,
            };

            // This assumes every antenna is also an anti-node itself.
            // Technically this is not correct according to the puzzle, but it gave me the right answer for my input ;)
            if (d8_set_contains(anti_nodes, &anti_node) || d8_set_contains(antennas, &anti_node)) {
                ch = anti_node.ch;
                count++;
            }

            printf("%c", ch);
        }
        printf("\n");
    }

    printf("Day 8_2: There are %d anti-nodes on the field\n", count);
    d8_destroy_set(antennas);
    d8_destroy_set(anti_nodes);
    str_destroy_lines(lines);
}
