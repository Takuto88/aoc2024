#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "liblists.h"

#define LOCATION_ID_LENGTH 5

static lists_t* create_list(int size) {
    lists_t* lists = malloc(sizeof(lists_t));
    if (lists == NULL) {
        return NULL;
    }

    lists->left = malloc(sizeof(int) * size);
    if(lists->left == NULL ) {
        free(lists);
        return NULL;
    }

    lists->right = malloc(sizeof(int) * size);
    if(lists->right == NULL) {
        free(lists->left);
        free(lists);
        return NULL;
    }

    return lists;
}

void destroy_list(lists_t* lists) {
    if(lists == NULL || lists->left == NULL || lists->right == NULL) {
        return;
    }

    free(lists->left);
    free(lists->right);
    free(lists);
}

lists_t* read_lists(const char* puzzle_input, int list_length) {
    FILE* fp = fopen(puzzle_input, "r");
    if(fp == NULL) {
        fprintf(stderr, "Could not open file: %s\n", puzzle_input);
        return NULL;
    }

    int ch;
    char currentNumber[LOCATION_ID_LENGTH + 1];
    int list_index = 0;
    int char_index = 0;
    lists_t* lists = create_list(list_length);
    bool whitespace_encountered = false;

    while(true) {
        ch = fgetc(fp);
        if (ch == ' ') {
            if (whitespace_encountered) {
                continue;
            }
            currentNumber[char_index] = 0;
            lists->left[list_index] = atoi(currentNumber);
            char_index = 0;
            whitespace_encountered = true;
            continue;
        }

        if (ch == '\n' || ch == EOF) {
            currentNumber[char_index] = 0;
            lists->right[list_index] = atoi(currentNumber);
            char_index = 0;
            list_index++;
            whitespace_encountered = false;
            if (ch == EOF) {
                break;
            }
            continue;
        }

        currentNumber[char_index] = ch;
        char_index++;
    }

    fclose(fp);

    return lists;
}