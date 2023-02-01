#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include "../includes/parser.h"
#include "../includes/users-catalog.h"
#include "../includes/queries.h"

Output_Type new_output_struct(bool interactive)
{
    Output_Type r = malloc(sizeof(struct output_type));
    if (interactive)
    {
        r->Type.str = NULL;
        r->interactive = true;
    }
    else
    {
        r->Type.fp = NULL;
        r->interactive = false;
    };

    return r;
}

void parse_line(const char *line, char **fields, char *delim)
{
    int index = 0;
    char *line_copy = NULL;
    char *token = NULL;

    line_copy = strdup(line); // save variable is fields[0]
    assert(line_copy != NULL);

    while ((token = strsep(&line_copy, delim)) != NULL)
        fields[index++] = token;
}

void parse_file(char *filename, int n_fields, int (*validation_function)(char **), void (*insertion_function)(char **, va_list), ...)
{
    FILE *fptr = NULL;
    fptr = fopen(filename, "r");

    if (fptr == NULL)
    {
        perror("Error: Failed to open data file!\n");
        exit(EXIT_FAILURE);
    }

    va_list args;

    char *line = NULL;
    size_t len = 0;
    ssize_t nread; /* number of characters read, including the delimiter character */

    nread = getline(&line, &len, fptr); // 1st line is disposable

    while ((nread = getline(&line, &len, fptr)) != -1)
    {
        char **fields = malloc(sizeof(char *) * n_fields);
        parse_line(line, fields, ";");

        if ((*validation_function)(fields))
        {
            va_start(args, insertion_function); // Reset argument list
            (*insertion_function)(fields, args);
        }

        free(fields[0]);
        free(fields);
    }

    va_end(args); // This is ok assuming the data file is not empty
    free(line);
    fclose(fptr);
}

void parse_query(char *query_path, int max_args, ...)
{
    FILE *input_file = NULL;
    input_file = fopen(query_path, "r");

    if (input_file == NULL)
    {
        perror("Error: Failed to open queries file!\n");
        exit(EXIT_FAILURE);
    }

    va_list args;

    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    char *output_path = malloc(40 * sizeof(char)); // 30 = path + \0, 10 = max integer value digits
    Output_Type output_struct = new_output_struct(false);
    int counter = 1;

    while ((nread = getline(&line, &len, input_file)) != -1)
    {
        char **fields = malloc(sizeof(char *) * max_args);
        parse_line(line, fields, " ");

        sprintf(output_path, "Resultados/command%d_output.txt", counter++);
        output_struct->Type.fp = fopen(output_path, "a");

        va_start(args, max_args); // Reset argument list
        handle_query(output_struct, fields, args);

        fclose(output_struct->Type.fp);
        free(fields[0]);
        free(fields);
    }

    va_end(args); // This is ok assuming the input file is not empty
    free(line);
    free(output_path);
    free(output_struct);
    fclose(input_file);
}

Output_Type parse_query_interactive(char *input, int max_args, va_list args)
{
    char **fields = malloc(sizeof(char *) * max_args);
    parse_line(input, fields, " ");
    Output_Type output_struct = new_output_struct(true);

    handle_query(output_struct, fields, args);

    free(fields[0]);
    free(fields);

    return output_struct;
}