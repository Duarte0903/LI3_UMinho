#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include "../includes/date.h"
#include "../includes/utils.h"

#define REF_DAY "9/10/2022"

char *get_age(unsigned short birth_date) {
    char *age_str = malloc(4 * sizeof(char));
    unsigned short ref_day = date_to_int(REF_DAY);
    unsigned short age = (ref_day - birth_date) / 365.25;
    sprintf(age_str, "%hu", age);
    return age_str;
}

char *get_file(char *path, const char *file) {
    char *result = malloc(strlen(path) + strlen(file) + 1);
    strcpy(result, path);
    strcat(result, file);
    return result;
}

int str_to_int(char *str) {
    char *end = NULL;
    errno = 0;
    int result = -1; // fix this
    long value = strtol(str, &end, 10);

    if (errno == 0 && *end == '\0') // Boundary check not needed since we are going to do an input validation
        result = (int)value;

    return result;
}

float str_to_float(char *str) {
    char *end = NULL;
    errno = 0;
    float result = -1.0; // fix this
    float value = strtof(str, &end);

    if (errno == 0 && *end == '\0') {
        result = value;
    }

    return result;
}

int first_occurrence_ptr_array_bsearch(GPtrArray *array, GCompareFunc compare_func, void *target, int search_bigger_nearest) {
    if (compare_func == NULL || array == NULL) {
        perror("Error: Binary search failed!\n");
        exit(EXIT_FAILURE);
    }

    int left = 0, right = array->len - 1, result = -1;
    int middle, value;
    void *elem;

    while (left <= right) {
        middle = left + (right - left) / 2;
        elem = g_ptr_array_index(array, middle);
        value = compare_func(&elem, target);

        if (value > 0)
            right = middle - 1;
        else if (value < 0)
            left = middle + 1;
        else {
            result = middle;
            right = middle - 1;
        }
    }

    if (search_bigger_nearest == 1 && result == -1 && left < (int)array->len)
        result = right + 1;

    return result;
}

int last_occurrence_ptr_array_bsearch(GPtrArray *array, GCompareFunc compare_func, void *target, int search_smaller_nearest) {
    if (compare_func == NULL || array == NULL) {
        perror("Error: Binary search failed!\n");
        exit(EXIT_FAILURE);
    }

    int left = 0, right = array->len - 1, result = -1;
    int middle, value;
    void *elem;

    while (left <= right) {
        middle = left + (right - left) / 2;
        elem = g_ptr_array_index(array, middle);
        value = compare_func(&elem, target);

        if (value > 0)
            right = middle - 1;
        else if (value < 0)
            left = middle + 1;
        else {
            result = middle;
            left = middle + 1;
        }
    }

    if (search_smaller_nearest == 1 && result == -1)
        result = left - 1;

    return result;
}
