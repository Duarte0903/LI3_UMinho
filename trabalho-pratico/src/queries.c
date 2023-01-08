#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include "../includes/queries.h"
#include "../includes/users-catalog.h"
#include "../includes/drivers-catalog.h"
#include "../includes/rides-catalog.h"
#include "../includes/date.h"
#include "../includes/utils.h"

void print_q1(FILE *output_file, char **fields, va_list args) {
    Users_Catalog users_catalog = va_arg(args, Users_Catalog);
    Drivers_Catalog drivers_catalog = va_arg(args, Drivers_Catalog);
    char *output = NULL;
    char *id = fields[1];
    id[strcspn(id, "\n")] = 0;

    if (isdigit(id[0])) // alterar o input handling
        output = get_driver_q1(id, drivers_catalog);
    else
        output = get_user_q1(id, users_catalog);

    if (output) {
        fprintf(output_file, "%s\n", output); // Optimize to fwrite?
        free(output);
    }
}

void print_q2(FILE *output_file, char **fields, va_list args) {
    (void)va_arg(args, Users_Catalog);
    Drivers_Catalog drivers_catalog = va_arg(args, Drivers_Catalog);
    char *output = NULL;
    char *n_drivers = fields[1];
    n_drivers[strcspn(n_drivers, "\n")] = 0;

    int n = str_to_int(n_drivers);
    int i = 0, index = 0;

    sort_drivers_by_average_rating(drivers_catalog); // otimizar para inactive accounts e com flag

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

void print_q3(FILE *output_file, char **fields, va_list args) {
    Users_Catalog users_catalog = va_arg(args, Users_Catalog);
    char *output = NULL;
    char *n_users = fields[1];
    n_users[strcspn(n_users, "\n")] = 0;

    int n = str_to_int(n_users);
    int i = 0, index = 0;

    sort_users_by_distance(users_catalog); // otimizar para ianctive accounts e com flag

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

void print_q4(FILE *output_file, char **fields, va_list args) {
    // Nothing to do
    (void)output_file;
    (void)fields;
    (void)args;
}

void print_q5(FILE *output_file, char **fields, va_list args) {
    (void)va_arg(args, Users_Catalog);
    (void)va_arg(args, Drivers_Catalog);
    Rides_Catalog rides_catalog = va_arg(args, Rides_Catalog);
    char *output = NULL;
    char *date1 = fields[1], *date2 = fields[2];
    date1[strcspn(date1, "\n")] = 0;
    date2[strcspn(date2, "\n")] = 0;

    unsigned short start_date = date_to_int(date1);
    unsigned short end_date = date_to_int(date2);

    if (end_date < start_date) // print empty file
        return;

    sort_rides_by_date(rides_catalog);
    
    output = get_q5(start_date, end_date, rides_catalog);

    if (output) {
        fprintf(output_file, "%s\n", output);
        free(output);
    }
}

void print_q6(FILE *output_file, char **fields, va_list args) {
    // Nothing to do
    (void)output_file;
    (void)fields;
    (void)args;
}

void print_q7(FILE *output_file, char **fields, va_list args) {
    // Nothing to do
    (void)output_file;
    (void)fields;
    (void)args;
}

void print_q8(FILE *output_file, char **fields, va_list args) {
    // Nothing to do
    (void)output_file;
    (void)fields;
    (void)args;
}

void print_q9(FILE *output_file, char **fields, va_list args) {
    // Nothing to do
    (void)output_file;
    (void)fields;
    (void)args;
}

void handle_query(FILE *output_file, char **fields, va_list args) {
    void (*query_selector[9])(FILE *, char **, va_list) = {print_q1, print_q2, print_q3, print_q4, print_q5, print_q6, print_q7, print_q8, print_q9};

    int query = str_to_int(fields[0]);

    (*query_selector[--query])(output_file, fields, args);
}
