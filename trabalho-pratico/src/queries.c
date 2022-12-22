#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include "../includes/queries.h"
#include "../includes/users-catalog.h"
#include "../includes/drivers-catalog.h"
#include "../includes/utils.h"

void print_q1(FILE *output_file, char *input, va_list args) {
    Users_Catalog users_catalog = va_arg(args, Users_Catalog);
    Drivers_Catalog drivers_catalog = va_arg(args, Drivers_Catalog);
    char *output = NULL;

    if (isdigit(input[0])) // alterar o input handling
        output = get_driver_q1(input, drivers_catalog);
    else
        output = get_user_q1(input, users_catalog);
    
    if (output)
        fprintf(output_file, "%s\n", output); // Optimize to fwrite?

    free(output);
}

void print_q2(FILE *output_file, char *input, va_list args) {
    (void)va_arg(args, Users_Catalog);
    Drivers_Catalog drivers_catalog = va_arg(args, Drivers_Catalog);
    char *output = NULL;
    int n = str_to_int(input);
    int i = 0, index = 0;

    sort_drivers_by_average_rating(drivers_catalog); //otimizar para inactive accounts

    while (i < n) {
        output = get_q2(index, drivers_catalog);

        if (output) {
            fprintf(output_file, "%s\n", output); // Optimize to fwrite?
            i++;
            free(output);
        }

        index++;
    }
}

void print_q3(FILE *output_file, char *input, va_list args) {
    Users_Catalog users_catalog = va_arg(args, Users_Catalog);
    char *output = NULL;
    int n = str_to_int(input);
    int i = 0, index = 0;

    sort_users_by_distance(users_catalog); // otimizar para ianctive accounts

    while (i < n) {
        output = get_q3(index, users_catalog);

        if (output) {
            fprintf(output_file, "%s\n", output); // Optimize to fwrite?
            i++;
            free(output);
        }

        index++;
    }
}

void print_empty_file(FILE *output_file, char *input, va_list args) {
    // Nothing to do
    (void)output_file;
    (void)input;
    (void)args;
}