#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <strings.h>
#include <string.h>
#include "../includes/drivers-catalog.h"
#include "../includes/driver.h"
#include "../includes/utils.h"
#include "../includes/date.h"
#include "../includes/vp_array.h"
#include "../includes/city_hash.h"

typedef struct drivers_catalog
{
    GHashTable *drivers_ht;
    GPtrArray *drivers_average_rating_arrays;
} *Drivers_Catalog;

typedef struct drivers_in_city
{
    GPtrArray *drivers_by_city;
    bool is_sorted;
} *Drivers_In_City;

void glib_wrapper_free_driver(gpointer driver)
{
    free_driver(driver);
}

void free_drivers_in_city(Drivers_In_City city)
{
    g_ptr_array_free(city->drivers_by_city, TRUE);
    free(city);
}

void glib_wrapper_free_drivers_in_city(gpointer city)
{
    free_drivers_in_city(city);
}

Drivers_In_City create_drivers_in_city()
{
    Drivers_In_City city = malloc(sizeof(struct drivers_in_city));

    city->drivers_by_city = g_ptr_array_new();
    city->is_sorted = false;

    return city;
}

void init_drivers_in_city(GPtrArray *arr, int capacity)
{
    for (int i = 0; i < capacity; i++)
    {
        Drivers_In_City city = create_drivers_in_city();
        g_ptr_array_add(arr, city);
    }
}

Drivers_Catalog create_drivers_catalog()
{
    Drivers_Catalog catalog = malloc(sizeof(struct drivers_catalog));

    catalog->drivers_ht = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, glib_wrapper_free_driver);
    catalog->drivers_average_rating_arrays = g_ptr_array_new_full(8, glib_wrapper_free_drivers_in_city);
    init_drivers_in_city(catalog->drivers_average_rating_arrays, 8);
    return catalog;
}

void add_driver_to_arrays(GPtrArray *arr, Driver driver, int capacity)
{
    for (int i = 0; i < capacity; i++)
    {
        Drivers_In_City drivers_in_city = g_ptr_array_index(arr, i);
        g_ptr_array_add(drivers_in_city->drivers_by_city, driver);
    }
}

int is_valid_driver(char **fields)
{
    if (IS_EMPTY(fields[0]) || IS_EMPTY(fields[1]) || IS_EMPTY(fields[3]) || IS_EMPTY(fields[5]) || IS_EMPTY(fields[6]))
        return 0;

    int day, month, year;

    if (!(sscanf(fields[2], "%2d/%2d/%4d", &day, &month, &year) == 3 && is_valid_date(day, month, year)))
        return 0;

    if (!(sscanf(fields[7], "%2d/%2d/%4d", &day, &month, &year) == 3 && is_valid_date(day, month, year)))
        return 0;

    if (IS_EMPTY(fields[4]) || (strcasecmp(fields[4], "basic") != 0 && strcasecmp(fields[4], "green") != 0 && strcasecmp(fields[4], "premium") != 0))
        return 0;

    if (IS_EMPTY(fields[8]) || (strcasecmp(fields[8], "active\n") != 0 && strcasecmp(fields[8], "inactive\n") != 0))
        return 0;

    return 1;
}

void insert_driver_in_catalog(char **fields, va_list args)
{
    Drivers_Catalog catalog = va_arg(args, Drivers_Catalog);
    Driver driver = create_driver(fields);
    char *key = get_driver_id(driver);
    add_driver_to_arrays(catalog->drivers_average_rating_arrays, driver, 8);

    g_hash_table_insert(catalog->drivers_ht, key, driver);
}

void update_driver_stats(char *driver_id, VPA *stats, Drivers_Catalog catalog)
{ // Improve function after input validation
    Driver driver = g_hash_table_lookup(catalog->drivers_ht, driver_id);
    set_driver_stats(driver, stats);
}

char *get_ride_car_class(char *driver_id, Drivers_Catalog catalog)
{
    Driver driver = g_hash_table_lookup(catalog->drivers_ht, driver_id);
    return get_driver_car_class(driver);
}

char *get_driver_name_id(char *driver_id, Drivers_Catalog catalog)
{
    Driver driver = g_hash_table_lookup(catalog->drivers_ht, driver_id);
    return get_driver_name(driver);
}

char *get_driver_gender_id(char *id, Drivers_Catalog catalog)
{
    Driver driver = g_hash_table_lookup(catalog->drivers_ht, id);
    return get_driver_gender(driver);
}

bool get_driver_account_status_id(char *driver_id, Drivers_Catalog catalog)
{
    Driver driver = g_hash_table_lookup(catalog->drivers_ht, driver_id);
    return get_driver_account_status(driver);
}

unsigned short get_driver_account_age_w_id(char *driver_id, Drivers_Catalog catalog)
{
    Driver driver = g_hash_table_lookup(catalog->drivers_ht, driver_id);
    return get_driver_account_age(driver);
}

static gint compare_drivers_by_average_rating(gconstpointer d1, gconstpointer d2)
{
    Driver driver1 = *(Driver *)d1;
    Driver driver2 = *(Driver *)d2;

    bool account_status1 = get_driver_account_status(driver1);
    bool account_status2 = get_driver_account_status(driver2);

    if (account_status1 && !account_status2)
        return -1;

    if (!account_status1 && account_status2)
        return 1;

    if (!account_status1 && !account_status2)
        return 0;

    double average_rating1 = get_driver_average_rating(driver1, 7);
    unsigned short date1 = get_driver_latest_ride(driver1);

    double average_rating2 = get_driver_average_rating(driver2, 7);
    unsigned short date2 = get_driver_latest_ride(driver2);

    int nearly_equal = nearly_equal_fp_numbers(average_rating1, average_rating2, 0.00001f);
    int result;
    // Descending order: average rating, date
    // Ascending order: driver_id
    if ((!nearly_equal && average_rating1 < average_rating2) || (nearly_equal && date1 < date2))
        result = 1;
    else if ((!nearly_equal && average_rating1 > average_rating2) || (nearly_equal && date1 > date2))
        result = -1;
    else
    {
        char *driver_id1 = get_driver_id(driver1);
        char *driver_id2 = get_driver_id(driver2);
        result = strcmp(driver_id1, driver_id2);
        free(driver_id1);
        free(driver_id2);
    }

    return result;
}

static gint compare_drivers_by_average_rating_in_city(gconstpointer d1, gconstpointer d2, gpointer city_index_ptr)
{
    Driver driver1 = *(Driver *)d1;
    Driver driver2 = *(Driver *)d2;
    int index = GPOINTER_TO_INT(city_index_ptr);

    char *driver_id1 = get_driver_id(driver1);
    char *driver_id2 = get_driver_id(driver2);

    double driver_average_rating1 = get_driver_average_rating(driver1, index);
    double driver_average_rating2 = get_driver_average_rating(driver2, index);

    int nearly_equal = nearly_equal_fp_numbers(driver_average_rating1, driver_average_rating2, 0.00001f);
    int result;

    if (!nearly_equal && driver_average_rating1 < driver_average_rating2)
        result = 1;
    else if (!nearly_equal && driver_average_rating1 > driver_average_rating2)
        result = -1;
    else
        result = -strcmp(driver_id1, driver_id2);

    free(driver_id1);
    free(driver_id2);

    return result;
}

void sort_drivers_by_average_rating(Drivers_Catalog catalog) // empty string if is total average rating
{
    Drivers_In_City drivers_in_city = g_ptr_array_index(catalog->drivers_average_rating_arrays, 7);
    if (!drivers_in_city->is_sorted)
    {
        g_ptr_array_sort(drivers_in_city->drivers_by_city, compare_drivers_by_average_rating);
        drivers_in_city->is_sorted = true;
    }
}

char *get_driver_q1(char *id, Drivers_Catalog catalog)
{ // change function and output variable name
    Driver driver = g_hash_table_lookup(catalog->drivers_ht, id);

    if (!driver || !get_driver_account_status(driver))
        return NULL;

    char *name = get_driver_name(driver);
    char *gender = get_driver_gender(driver);
    char *age = get_driver_age(driver);
    double average_rating = get_driver_average_rating(driver, 7);
    unsigned short total_rides = get_driver_total_rides(driver);
    double total_earned = get_driver_total_earned_money(driver);

    char *driver_str = malloc(strlen(name) + strlen(gender) + strlen(age) + 5 + 10 + 10 + 5 + 1); // 5 de rating, 10 de total_rides, 10 de money, 5 dos ;, 1 do \0
    sprintf(driver_str, "%s;%s;%s;%.3f;%hu;%.3f", name, gender, age, average_rating, total_rides, total_earned);

    free(name);
    free(gender);
    free(age);

    return driver_str;
}

char *get_q2(int n_drivers, Drivers_Catalog catalog)
{

    Drivers_In_City drivers_in_city = g_ptr_array_index(catalog->drivers_average_rating_arrays, 7); // 7 is total average rating

    if (n_drivers > (int)drivers_in_city->drivers_by_city->len)
        n_drivers = drivers_in_city->drivers_by_city->len;

    char *result = NULL;
    size_t result_size = 0;
    FILE *stream = open_memstream(&result, &result_size);
    char *driver_id = NULL, *name = NULL;
    double average_rating;

    for (int i = 0; i < n_drivers; i++)
    {
        Driver driver = g_ptr_array_index(drivers_in_city->drivers_by_city, i);
        if (get_driver_account_status(driver))
        {
            driver_id = get_driver_id(driver);
            name = get_driver_name(driver);
            average_rating = get_driver_average_rating(driver, 7);
            fprintf(stream, "%s;%s;%0.3f\n", driver_id, name, average_rating);
            free(driver_id);
            free(name);
        }
        else if (n_drivers < (int)drivers_in_city->drivers_by_city->len)
            n_drivers++;
    }

    fclose(stream);

    return result;
}

char *get_q7(int n_drivers, char *city, Drivers_Catalog catalog)
{
    int city_index = get_city_index(city);
    if (city_index == -1)
        return NULL;

    Drivers_In_City drivers_in_city = g_ptr_array_index(catalog->drivers_average_rating_arrays, city_index);
    if (!drivers_in_city->is_sorted)
    {
        g_ptr_array_sort_with_data(drivers_in_city->drivers_by_city, compare_drivers_by_average_rating_in_city, GINT_TO_POINTER(city_index));
        drivers_in_city->is_sorted = true;
    }

    if (n_drivers > (int)drivers_in_city->drivers_by_city->len)
        n_drivers = drivers_in_city->drivers_by_city->len;

    char *result = NULL;
    size_t result_size = 0;
    FILE *stream = open_memstream(&result, &result_size);

    for (int i = 0; i < n_drivers; i++)
    {
        Driver driver = g_ptr_array_index(drivers_in_city->drivers_by_city, i);
        if (get_driver_account_status(driver))
        {
            char *id = get_driver_id(driver);
            char *name = get_driver_name(driver);
            double average_rating = get_driver_average_rating(driver, city_index);
            fprintf(stream, "%s;%s;%.3f\n", id, name, average_rating);
            free(id);
            free(name);
        }
        else if (n_drivers < (int)drivers_in_city->drivers_by_city->len)
            n_drivers++;
    }
    fclose(stream);
    return result;
}

void free_drivers_catalog(Drivers_Catalog catalog)
{
    g_hash_table_destroy(catalog->drivers_ht);
    g_ptr_array_free(catalog->drivers_average_rating_arrays, TRUE);
    free(catalog);
}
