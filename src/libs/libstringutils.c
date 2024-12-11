#include <stdbool.h>
#include <stddef.h>
#include "libstringutils.h"


int count_char(const char* line, const char c) {
    int count = 0;
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == c) {
            count++;
        }
    }

    return count;
}

int str_contains_times(const char* haystack, const size_t haystack_size, const char* needle, const size_t needle_size) {
    int count = 0;

    for(int i = 0; i < haystack_size; i++) {
        if (i + needle_size > haystack_size) {
            break;
        }

        bool found_char = false;
        for(int y = 0; y < needle_size; y++) {
            found_char = haystack[i] == needle[y];
            if(!found_char) {
                break;
            }
        }

        if(found_char) {
            count++;
        }
    }

    return count;
}