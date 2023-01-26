#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <glib.h>
#include "../includes/queries.h"
#include "../includes/users-catalog.h"
#include "../includes/drivers-catalog.h"
#include "../includes/rides-catalog.h"
#include "../includes/date.h"
#include "../includes/utils.h"

void print_q1(FILE *output_file, char **fields, va_list args)
{
    Users_Catalog users_catalog = va_arg(args, Users_Catalog);
    Drivers_Catalog drivers_catalog = va_arg(args, Drivers_Catalog);
    char *output = NULL;
    char *id = fields[1];
    id[strcspn(id, "\n")] = 0;

    if (isdigit(id[0])) // alterar o input handling
        output = get_driver_q1(id, drivers_catalog);
    else
        output = get_user_q1(id, users_catalog);

    if (output)
    {
        fprintf(output_file, "%s\n", output); // Optimize to fwrite?
        free(output);
    }
}

void print_q2(FILE *output_file, char **fields, va_list args)
{
    (void)va_arg(args, Users_Catalog);
    Drivers_Catalog drivers_catalog = va_arg(args, Drivers_Catalog);
    char *output = NULL;
    char *n_drivers = fields[1];
    n_drivers[strcspn(n_drivers, "\n")] = 0;

    int n = str_to_int(n_drivers);
    int i = 0, index = 0;

    sort_drivers_by_average_rating(drivers_catalog); // otimizar para inactive accounts e com flag

    while (i < n)
    {
        output = get_q2(index, drivers_catalog);

        if (output)
        {
            fprintf(output_file, "%s\n", output); // Optimize to fwrite?
            i++;
            free(output);
        }

        index++;
    }
}

void print_q3(FILE *output_file, char **fields, va_list args)
{
    Users_Catalog users_catalog = va_arg(args, Users_Catalog);
    char *output = NULL;
    char *n_users = fields[1];
    n_users[strcspn(n_users, "\n")] = 0;

    int n = str_to_int(n_users);
    int i = 0, index = 0;

    sort_users_by_distance(users_catalog); // otimizar para ianctive accounts e com flag

    while (i < n)
    {
        output = get_q3(index, users_catalog);

        if (output)
        {
            fprintf(output_file, "%s\n", output); // Optimize to fwrite?
            i++;
            free(output);
        }

        index++;
    }
}

void print_q4(FILE *output_file, char **fields, va_list args)
{
    (void)va_arg(args, Users_Catalog);
    (void)va_arg(args, Drivers_Catalog);
    Rides_Catalog rides_catalog = va_arg(args, Rides_Catalog);
    char *output = NULL;
    char *city = fields[1];
    city[strcspn(city, "\n")] = 0;

    sort_rides_by_city(rides_catalog);
    output = get_q4(city, rides_catalog);

    if (output)
    {
        fprintf(output_file, "%s\n", output);
        free(output);
    }
}

void print_q5(FILE *output_file, char **fields, va_list args)
{
    (void)va_arg(args, Users_Catalog);
    (void)va_arg(args, Drivers_Catalog);
    Rides_Catalog rides_catalog = va_arg(args, Rides_Catalog);
    char *output = NULL;
    char *date1 = fields[1], *date2 = fields[2];
    date2[strcspn(date2, "\n")] = 0;

    unsigned short start_date = date_to_int(date1);
    unsigned short end_date = date_to_int(date2);

    if (end_date < start_date) // print empty file
        return;

    sort_rides_by_date(rides_catalog);

    output = get_q5(start_date, end_date, rides_catalog);

    if (output)
    {
        fprintf(output_file, "%s\n", output);
        free(output);
    }
}

void print_q6(FILE *output_file, char **fields, va_list args)
{
    (void)va_arg(args, Users_Catalog);
    (void)va_arg(args, Drivers_Catalog);
    Rides_Catalog rides_catalog = va_arg(args, Rides_Catalog);
    char *output = NULL;
    char *city = fields[1], *date1 = fields[2], *date2 = fields[3];
    date2[strcspn(date2, "\n")] = 0;

    unsigned short start_date = date_to_int(date1);
    unsigned short end_date = date_to_int(date2);

    if (end_date < start_date) // print empty file
        return;

    sort_rides_by_date(rides_catalog);

    output = get_q6(city, start_date, end_date, rides_catalog);

    if (output)
    {
        fprintf(output_file, "%s\n", output);
        free(output);
    }
}

void print_q7(FILE *output_file, char **fields, va_list args)
{
    va_list args_copy;
    va_copy(args_copy, args);

    (void)va_arg(args, Users_Catalog);
    (void)va_arg(args, Drivers_Catalog);
    Rides_Catalog rides_catalog = va_arg(args, Rides_Catalog);

    int output_number = str_to_int(fields[1]);
    char *city = fields[2];
    city[strcspn(city, "\n")] = 0;

    sort_rides_by_city_and_driver_id(rides_catalog);

    char *output = get_q7(output_number, city, args_copy);

    if (output)
    {
        fprintf(output_file, "%s\n", output);
        free(output);
    }

    va_end(args_copy);
}

void print_q8(FILE *output_file, char **fields, va_list args)
{

    Users_Catalog users_catalog = va_arg(args, Users_Catalog);
    Drivers_Catalog drivers_catalog = va_arg(args, Drivers_Catalog);
    Rides_Catalog rides_catalog = va_arg(args, Rides_Catalog);

    char *gender = fields[1];
    fields[2][strcspn(fields[2], "\n")] = '\0';

    int minimum_age = str_to_int(fields[2]);

    GPtrArray *catalog_pointers_extra_data = g_ptr_array_new();
    g_ptr_array_add(catalog_pointers_extra_data, users_catalog);
    g_ptr_array_add(catalog_pointers_extra_data, drivers_catalog);

    sort_rides_by_account_age(rides_catalog, catalog_pointers_extra_data);

    char *output = get_q8(gender, minimum_age, rides_catalog, catalog_pointers_extra_data);

    if (output)
    {
        fprintf(output_file, "%s\n", output);
        free(output);
    }

    g_ptr_array_free(catalog_pointers_extra_data, TRUE);
}

void print_q9(FILE *output_file, char **fields, va_list args)
{
    (void)va_arg(args, Users_Catalog);
    (void)va_arg(args, Drivers_Catalog);
    Rides_Catalog rides_catalog = va_arg(args, Rides_Catalog);
    char *output = NULL;
    char *date1 = fields[1], *date2 = fields[2];
    date2[strcspn(date2, "\n")] = 0;

    unsigned short start_date = date_to_int(date1);
    unsigned short end_date = date_to_int(date2);

    if (end_date < start_date) // print empty file
        return;

    sort_rides_by_date(rides_catalog);

    int first_date = get_rides_first_date(rides_catalog, start_date);
    if (first_date == -1)
        return;
    int last_date = get_rides_last_date(rides_catalog, end_date);

    GPtrArray *results_array = g_ptr_array_new();

    copy_rides_to_results_array(rides_catalog, first_date, last_date, results_array);

    sort_rides_by_distance(results_array);

    int last_index = get_last_ride_w_nonzero_tip(results_array);

    if (last_index == -1)
        last_index = 0;

    int index = 0;

    while (index <= last_index)
    {
        output = get_q9(index, results_array);

        if (output)
        {
            fprintf(output_file, "%s\n", output); // Optimize to fwrite?
            free(output);
        }

        index++;
    }
}

void handle_query(FILE *output_file, char **fields, va_list args)
{
    void (*query_selector[9])(FILE *, char **, va_list) = {print_q1, print_q2, print_q3, print_q4, print_q5, print_q6, print_q7, print_q8, print_q9};

    int query = str_to_int(fields[0]);

    (*query_selector[--query])(output_file, fields, args);
}