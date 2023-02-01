#ifndef PARSER_H
#define PARSER_H

typedef struct output_type
{
    union type
    {
        FILE *fp;
        char *str;
    } Type;
    bool interactive;
} *Output_Type;

void parse_file(char *filename, int n_fields, int (*validation_function)(char **), void (*insertion_function)(char **, va_list), ...);

void parse_query(char *query_path, int max_args, ...);

Output_Type parse_query_interactive(char *input, int max_args, va_list args);

#endif