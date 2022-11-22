#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../includes/utils.h"
#include "../includes/catalog.h"
#include "../includes/queries.h"

void parse_line(const char *line, char **fields, char *delim)
{
    int index = 0;
    char *line_copy = NULL;
    char *token = NULL;

    line_copy = strdup(line); // save variable is fields[0]
    assert(line_copy != NULL);

    while ((token = strsep(&line_copy, delim)) != NULL) // don't need to compare n_fields with index (input validation)
        fields[index++] = token;
}

void parse_file(char *filename, int n_fields, void (*function_ptr)(char **, Catalog), Catalog catalog)
{
    FILE *fptr = NULL;
    fptr = fopen(filename, "r");

    if (fptr == NULL)
    {
        perror("Error!");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t nread; /* number of characters read, including the delimiter character */

    nread = getline(&line, &len, fptr); // 1st line is disposable

    while ((nread = getline(&line, &len, fptr)) != -1)
    {
        char **fields = malloc(sizeof(char *) * n_fields);
        parse_line(line, fields, ";");
        // debug_parser(fields, n_fields);
        (*function_ptr)(fields, catalog);
        free(fields[0]);
        free(fields);
    }
    free(line);
    fclose(fptr);
}

void parse_query(char *query_path, int max_args, Catalog catalog)
{
    FILE *fptr = NULL;
    fptr = fopen(query_path, "r");

    if (fptr == NULL)
    {
        perror("Error!");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    int counter = 1;

    while ((nread = getline(&line, &len, fptr)) != -1)
    {
        char **fields = malloc(sizeof(char *) * max_args);
        parse_line(line, fields, " ");
        handle_query(fields, catalog, counter);
        counter++;
        free(fields[0]);
        free(fields);
    }
    free(line);
    fclose(fptr);
}