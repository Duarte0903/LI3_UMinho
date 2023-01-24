#ifndef PARSER_H
#define PARSER_H

void parse_file(char *filename, int n_fields, int (*validation_function)(char **), void (*insertion_function)(char **, va_list), ...);

void parse_query(char *query_path, int max_args, ...);

#endif