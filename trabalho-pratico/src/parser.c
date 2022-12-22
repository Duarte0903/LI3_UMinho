#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include "../includes/users-catalog.h"
#include "../includes/queries.h"
#include "../includes/utils.h"


void parse_line(const char *line, char **fields, char *delim) {
    int index = 0;
    char *line_copy = NULL;
    char *token = NULL;

    line_copy = strdup(line); // save variable is fields[0]
    assert(line_copy != NULL);

    while ((token = strsep(&line_copy, delim)) != NULL) // don't need to compare n_fields with index (input validation)
        fields[index++] = token;
}

void parse_file(char *filename, int n_fields, void (*insert_function)(char **, va_list), ...) {
    FILE *fptr = NULL;
    fptr = fopen(filename, "r");

    if (fptr == NULL) {
        perror("Error: Failed to open data file!\n");
        exit(EXIT_FAILURE);
    }

    va_list args;

    char *line = NULL;
    size_t len = 0;
    ssize_t nread; /* number of characters read, including the delimiter character */

    nread = getline(&line, &len, fptr); // 1st line is disposable

    while ((nread = getline(&line, &len, fptr)) != -1) {
        char **fields = malloc(sizeof(char *) * n_fields);
        parse_line(line, fields, ";");

        va_start(args, insert_function);
        (*insert_function)(fields, args);
        
        free(fields[0]);
        free(fields);
    }
    
    va_end(args); // This is ok assuming the data file is not empty
    free(line);
    fclose(fptr);
}

void parse_query(char *query_path, int max_args, ...) {
    FILE *input_file = NULL;
    input_file = fopen(query_path, "r");

    if (input_file == NULL) {
        perror("Error: Failed to open queries file!\n");
        exit(EXIT_FAILURE);
    }

    va_list args;

    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    void (*query_selector[4])(FILE *, char *, va_list) = {print_q1, print_q2, print_q3, print_empty_file};
    char *output_path = malloc(40 * sizeof(char)); // 30 = path + \0, 10 = max integer value digits
    FILE *output_file = NULL; // dar handle de erros para alocaçao de memoria e abertura de ficheiros
    int counter = 1, query;

    while ((nread = getline(&line, &len, input_file)) != -1) {
        char **fields = malloc(sizeof(char *) * max_args);
        parse_line(line, fields, " ");

        query = str_to_int(fields[0]);
        query = (query > 0 && query < 4) ? (query - 1) : 3;
        fields[1][strcspn(fields[1], "\n")] = 0;

        sprintf(output_path, "Resultados/command%d_output.txt", counter++);
        output_file = fopen(output_path, "a");

        va_start(args, max_args);
        (*query_selector[query])(output_file, fields[1], args); // tem que ser alterado nas proximas queries que têm mais de 1 argumento
        // isto está mal feito eu devia ter posto este array de function pointers nas queries para as encapsular
        fclose(output_file);
        free(fields[0]);
        free(fields);
    }

    va_end(args);
    free(line);
    free(output_path);
    fclose(input_file);
}