#ifndef PARSER_H
#define PARSER_H

void parse_file(char *filename, int n_fields, void (*function_ptr)(char **, va_list), ...);

void parse_query(char *query_path, int max_args, ...);

#endif