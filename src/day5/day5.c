#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "day5.h"


ordering_rule_t *parse_ordering_rule(const char *line) {
    char *line_copy = strdup(line);
    if (line_copy == NULL) {
        return NULL;
    }

    char *line_copy_ptr = line_copy;

    ordering_rule_t *rule = malloc(sizeof(ordering_rule_t));
    if (rule == NULL) {
        free(line_copy);
        return NULL;
    }

    char *low = strsep(&line_copy, "|");
    char *high = strsep(&line_copy, "\n");

    if (high[0] == '\0') {
        free(line_copy_ptr);
        free(rule);
        return NULL;
    }

    char *low_errors, *high_errors;
    rule->low = strtol(low, &low_errors, 10);
    rule->high = strtol(high, &high_errors, 10);

    if (*low_errors != '\0' || *high_errors != '\0') {
        free(line_copy_ptr);
        free(rule);
        return NULL;
    }

    free(line_copy_ptr);

    return rule;
}

void destroy_rules(ordering_rule_t **rules, const size_t rules_size) {
    if (rules == NULL) {
        return;
    }

    for (size_t i = 0; i < rules_size; i++) {
        if (rules[i] == NULL) {
            continue;
        }

        free(rules[i]);
    }

    free(rules);
}

ordering_rule_t **read_rules(FILE *fp, size_t *returned_size) {
    ordering_rule_t **rules = NULL;
    size_t rules_size = 0;

    char *line = NULL;
    size_t line_size = 0;
    while (getline(&line, &line_size, fp) != -1) {
        if (strstr(line, "|") == NULL) {
            break;
        }

        ordering_rule_t *rule = parse_ordering_rule(line);
        if (rule == NULL) {
            fprintf(stderr, "ABORT: Failed to parse rule: %s\n", line);
            abort();
        }

        ordering_rule_t **new_rules_buffer = realloc(rules, sizeof(ordering_rule_t *) * (rules_size + 1));
        if (new_rules_buffer == NULL) {
            destroy_rules(rules, rules_size);
            free(line);
            return NULL;
        }

        rules = new_rules_buffer;
        rules[rules_size] = rule;
        rules_size++;
    }

    free(line);
    *returned_size = rules_size;

    return rules;
}

void destroy_page_update(page_update_t *update) {
    if (update == NULL) {
        return;
    }

    if (update->updates != NULL) {
        free(update->updates);
    }

    free(update);
}

void destroy_page_updates(page_update_t **updates, size_t updates_size) {
    if (updates == NULL) {
        return;
    }

    for (size_t i = 0; i < updates_size; i++) {
        destroy_page_update(updates[i]);
    }

    free(updates);
}

page_update_t *parse_page_update(const char *line) {
    char *line_copy = strdup(line);
    if (line_copy == NULL) {
        return NULL;
    }

    char *line_copy_ptr = line_copy;

    page_update_t *update = malloc(sizeof(page_update_t));
    if (update == NULL) {
        free(line_copy_ptr);
        return NULL;
    }

    update->updates_size = 0;
    update->updates = malloc(sizeof(long) * 1);

    char *page_str;
    while ((page_str = strsep(&line_copy, ",")) != NULL) {
        if (page_str[0] == '\n') {
            break;
        }

        char *page_errors;
        long page = strtol(page_str, &page_errors, 10);
        if (*page_errors != '\0' && *page_errors != '\n') {
            free(line_copy_ptr);
            destroy_page_update(update);
            return NULL;
        }

        long *new_updates_buffer = realloc(update->updates, sizeof(long) * (update->updates_size + 1));
        if (new_updates_buffer == NULL) {
            free(line_copy_ptr);
            destroy_page_update(update);
            return NULL;
        }

        update->updates = new_updates_buffer;
        update->updates[update->updates_size] = page;
        update->updates_size++;
    }

    update->middle = update->updates[update->updates_size / 2];
    free(line_copy_ptr);

    return update;
}

page_update_t **read_page_updates(FILE *fp, size_t *returned_size) {
    page_update_t **updates = NULL;
    size_t updates_size = 0;

    char *line = NULL;
    size_t line_size = 0;
    while (getline(&line, &line_size, fp) != -1) {
        page_update_t *update = parse_page_update(line);
        if (update == NULL) {
            fprintf(stderr, "ABORT: Failed to parse update: %s\n", line);
            abort();
        }

        page_update_t **new_updates_buffer = realloc(updates, sizeof(page_update_t *) * (updates_size + 1));
        if (new_updates_buffer == NULL) {
            for (size_t i = 0; i < updates_size && updates != NULL; i++) {
                destroy_page_update(updates[i]);
            }

            if (updates != NULL) {
                free(updates);
            }
            free(line);
            return NULL;
        }

        updates = new_updates_buffer;
        updates[updates_size] = update;
        updates_size++;
    }

    free(line);
    *returned_size = updates_size;

    return updates;
}

ordering_rule_t **find_rules_for_page_update(ordering_rule_t **ordering_rules, size_t input_size,
                                             size_t *output_size, const page_update_t *page_update) {
    if (input_size == 0 || ordering_rules == NULL || page_update == NULL) {
        *output_size = 0;
        return NULL;
    }

    size_t output_idx = 0;
    ordering_rule_t **matching_rules = calloc(input_size, sizeof(ordering_rule_t));

    for (size_t i = 0; i < input_size; i++) {
        ordering_rule_t *current_rule = ordering_rules[i];

        long matched_low = 0;
        long matched_high = 0;
        for (size_t y = 0; y < page_update->updates_size; y++) {
            const long current_update = page_update->updates[y];
            if (current_update == current_rule->low) {
                matched_low = current_rule->low;
                continue;
            }

            if (current_update == current_rule->high) {
                matched_high = current_rule->high;
            }
        }

        if (matched_low && matched_high) {
            matching_rules[output_idx] = current_rule;
            output_idx++;
        }
    }

    *output_size = output_idx;
    ordering_rule_t **output_buffer = realloc(matching_rules, sizeof(ordering_rule_t) * output_idx);
    if (output_buffer == NULL) {
        *output_size = 0;
        free(matching_rules);
        return NULL;
    }

    matching_rules = output_buffer;
    return matching_rules;
}

void solve_day5_1(const char *puzzle_input) {
    FILE *fp = fopen(puzzle_input, "r");
    size_t rules_size = 0;
    ordering_rule_t **rules = read_rules(fp, &rules_size);

    for (size_t i = 0; i < rules_size; i++) {
        printf("Rule %zu: %ld | %ld\n", i, rules[i]->low, rules[i]->high);
    }

    size_t updates_size = 0;
    page_update_t **updates = read_page_updates(fp, &updates_size);

    for (size_t i = 0; i < updates_size; i++) {
        const page_update_t *current_update = updates[i];
        size_t matching_rules_size = 0;

        ordering_rule_t **matching_rules = find_rules_for_page_update(rules, rules_size, &matching_rules_size, current_update);

        printf("Update %zu: ", i);
        for (size_t j = 0; j < current_update->updates_size; j++) {
            printf("%ld, ", current_update->updates[j]);
        }
        printf("Middle: %ld, ", current_update->middle);

        printf("Matched rules: ");
        for (size_t j = 0; j < matching_rules_size; j++) {
            printf("(%ld|%ld),", matching_rules[j]->low, matching_rules[j]->high);
        }
        printf("\n");
        free(matching_rules);
    }

    fclose(fp);
    destroy_rules(rules, rules_size);
    destroy_page_updates(updates, updates_size);
}
