#ifndef UTILS_H
#define UTILS_H

#include <glib.h>

char* get_age(unsigned short birth_day);

unsigned short get_age_no_ref(unsigned short birth_date);

char *get_file(char *path, const char *file);

int str_to_int(char *str);

float str_to_float(char *str);

int first_occurrence_ptr_array_bsearch(GPtrArray *array, GCompareFunc compare_func, void *target, int search_bigger_nearest);

int last_occurrence_ptr_array_bsearch(GPtrArray *array, GCompareFunc compare_func, void *target, int search_smaller_nearest);

int first_occurrence_ptr_array_bsearch_with_data(GPtrArray *array, GCompareDataFunc compare_func, void *target, int search_smaller_nearest, gpointer extra_data);

int last_occurrence_ptr_array_bsearch_with_data(GPtrArray *array, GCompareDataFunc compare_func, void *target, int search_smaller_nearest, gpointer extra_data);

#endif