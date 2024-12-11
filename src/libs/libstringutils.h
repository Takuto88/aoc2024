#ifndef AOC_2024_LIBSTRINGUTILS_H
#define AOC_2024_LIBSTRINGUTILS_H
#include <stddef.h>

/**
 * \brief A structure to hold an array of strings.
 */
typedef struct {
    char **lines;
    size_t *line_len;
    size_t num_lines;
} str_lines_t;

/**
 * \brief Counts the number of times any character from the needle appears in the haystack.
 *
 * \param haystack The string to search within.
 * \param haystack_size The size of the haystack string.
 * \param needle The string containing characters to search for.
 * \param needle_size The size of the needle string.
 * \return The number of times any character from the needle appears in the haystack.
 */
int str_contains_times(const char* haystack, size_t haystack_size, const char* needle, size_t needle_size);

/**
 * \brief Counts the number of occurrences of a specific character in a string.
 *
 * \param line The string to search within.
 * \param c The character to count.
 * \return The number of times the character appears in the string.
 */
int str_count_char(const char* line, char c);int str_array_idx(const char);

/**
 * \brief Reads all lines from a file and stores them in a str_lines_t structure.
 *
 * \param file The path to the file to read.
 * \return A pointer to a str_lines_t structure containing the lines read from the file.
 *         Returns NULL if the file could not be opened or if there was an error allocating memory.
 */
str_lines_t *str_read_lines(const char *file);void str_destroy_lines(str_lines_t *lines);

#endif //AOC_2024_LIBSTRINGUTILS_H
