#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../includes/queries.h"
#include "../includes/utils.h"
#include "../includes/catalog.h"

void print_q1(char *input, Catalog catalog, int counter)
{
    char *output = NULL;
    char *file_path = malloc(75 * sizeof(char));
    sprintf(file_path, "Resultados/command%d_output.txt", counter);
    if (isdigit(input[0]))
    {
        output = get_driver_q1(input, catalog);
    }
    else
    {
        output = get_user_q1(input, catalog);
    }
    FILE *results = fopen(file_path, "a");
    
    if (output)
        fprintf(results, "%s\n", output);

    fclose(results);
    free(output);
    free(file_path);
}

void print_q2(char *input, Catalog catalog, int counter)
{
    char *output = NULL;
    char *file_path = malloc(75 * sizeof(char));
    sprintf(file_path, "Resultados/command%d_output.txt", counter);
    int n = str_to_int(input);
    int i = 0, index = 0;

    sort_q2(catalog);

    FILE *results = fopen(file_path, "a");

    while (i < n)
    {
        output = get_q2(index, catalog);

        if (output)
        {
            fprintf(results, "%s\n", output);
            i++;
            free(output);
        }

        index++;
    }

    fclose(results);
    free(file_path);
}

void print_q3(char *input, Catalog catalog, int counter)
{
    char *output = NULL;
    char *file_path = malloc(75 * sizeof(char));
    sprintf(file_path, "Resultados/command%d_output.txt", counter);
    int n = str_to_int(input);
    int i = 0, index = 0;

    sort_q3(catalog);

    FILE *results = fopen(file_path, "a");

    while (i < n)
    {
        output = get_q3(index, catalog);

        if (output)
        {
            fprintf(results, "%s\n", output);
            i++;
            free(output);
        }

        index++;
    }

    fclose(results);
    free(file_path);
}

void print_empty_file(int counter)
{
    char *file_path = malloc(75 * sizeof(char));
    sprintf(file_path, "Resultados/command%d_output.txt", counter);

    FILE *results = fopen(file_path, "a");
    fclose(results);

    free(file_path);
}

void handle_query(char **fields, Catalog catalog, int counter)
{
    int query_number = str_to_int(fields[0]);

    char *args = fields[1];
    args[strcspn(args, "\n")] = '\0';

    if (query_number == 1)
    {
        print_q1(args, catalog, counter);
    }
    else if (query_number == 2)
    {
        print_q2(args, catalog, counter);
    }
    else if (query_number == 3)
    {
        print_q3(args, catalog, counter);
    }
    else
        print_empty_file(counter);
}