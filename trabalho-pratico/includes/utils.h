#ifndef UTILS_H
#define UTILS_H

#include <glib.h>

#define IS_EMPTY(str) (!str[0])

char* get_age(unsigned short birth_day);

char *get_file(char *path, const char *file);

int is_positive_integer(char *str);

int is_non_negative_float(char *str);

int str_to_int(char *str);

float str_to_float(char *str);

int nearly_equal_floats(float f1, float f2, float epsilon);

int first_occurrence_ptr_array_bsearch(GPtrArray *array, GCompareFunc compare_func, void *target, int search_bigger_nearest);

int last_occurrence_ptr_array_bsearch(GPtrArray *array, GCompareFunc compare_func, void *target, int search_smaller_nearest);

#endif