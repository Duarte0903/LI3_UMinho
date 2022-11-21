#ifndef UTILS_H
#define UTILS_H

// char **parse_line(char *line, int n_elems);

void free_str_array(char **arr, int n);

// char *strcat_driver_data(char **strArr);

// char *strcat_user_data(char **strArr);

// char *strcat_user_data_q3(char **strArr);

int str_to_int(char *str);

void init_str_to_0(char *str);

char *get_file(char *path, const char *file);

void debug_parser(char **arr, int n);

float str_to_float(char *str);

char* get_age(unsigned short birth_day);

#endif