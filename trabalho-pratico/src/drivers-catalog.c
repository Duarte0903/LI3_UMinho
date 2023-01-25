#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <strings.h>
#include <string.h>
#include "../includes/drivers-catalog.h"
#include "../includes/driver.h"
#include "../includes/utils.h"
#include "../includes/date.h"

typedef struct drivers_catalog
{
    GPtrArray *drivers_array;
    GHashTable *drivers_ht;
} *Drivers_Catalog;

void glib_wrapper_free_driver(gpointer driver)
{
    free_driver(driver);
}

Drivers_Catalog create_drivers_catalog()
{
    Drivers_Catalog catalog = malloc(sizeof(struct drivers_catalog));

    catalog->drivers_array = g_ptr_array_new(); // update to ptr array full?
    catalog->drivers_ht = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, glib_wrapper_free_driver);

    return catalog;
}

int is_valid_driver(char **fields) {
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

void insert_driver_in_catalog(char **fields, va_list args) {
    Drivers_Catalog catalog = va_arg(args, Drivers_Catalog);
    Driver driver = create_driver(fields);
    char *key = get_driver_id(driver);

    g_hash_table_insert(catalog->drivers_ht, key, driver);
    g_ptr_array_add(catalog->drivers_array, driver);
}

void update_driver_stats(char *driver_id, void **stats, Drivers_Catalog catalog)
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

bool get_driver_account_status_id(char *driver_id, Drivers_Catalog catalog){
    Driver driver = g_hash_table_lookup(catalog->drivers_ht, driver_id);
    return get_driver_account_status(driver);
}

static gint compare_drivers_by_average_rating(gconstpointer d1, gconstpointer d2)
{ // optimize multiple ifs to 1 if
    int result;

    Driver driver1 = *(Driver *)d1;
    Driver driver2 = *(Driver *)d2;

    float average_rating1 = get_driver_average_rating(driver1);
    unsigned short date1 = get_driver_latest_ride(driver1);
    char *driver_id1 = get_driver_id(driver1);

    float average_rating2 = get_driver_average_rating(driver2);
    unsigned short date2 = get_driver_latest_ride(driver2);
    char *driver_id2 = get_driver_id(driver2);

    int nearly_equal = nearly_equal_floats(average_rating1, average_rating2, 0.00001f);

    if ((!nearly_equal && average_rating1 < average_rating2) || (nearly_equal && date1 < date2))
        result = 1;

    if ((!nearly_equal && average_rating1 > average_rating2) || (nearly_equal && date1 > date2))
        result = -1;

    if (nearly_equal && date1 == date2)
        result = strcmp(driver_id1, driver_id2);

    free(driver_id1);
    free(driver_id2);

    return result;
}

void sort_drivers_by_average_rating(Drivers_Catalog catalog)
{ // add flag to check if sorted
    g_ptr_array_sort(catalog->drivers_array, compare_drivers_by_average_rating);
}

char *get_driver_q1(char *id, Drivers_Catalog catalog)
{ // change function and output variable name
    Driver driver = g_hash_table_lookup(catalog->drivers_ht, id);

    if (!driver || !get_driver_account_status(driver))
        return NULL;

    char *name = get_driver_name(driver);
    char *gender = get_driver_gender(driver);
    char *age = get_driver_age(driver);
    double average_rating = get_driver_average_rating(driver);
    unsigned short total_rides = get_driver_total_rides(driver);
    double total_earned = get_driver_total_earned_money(driver);

    char *driver_str = malloc(strlen(name) + strlen(gender) + strlen(age) + 5 + 10 + 10 + 5 + 1); // 5 de rating, 10 de total_rides, 10 de money, 5 dos ;, 1 do \0
    sprintf(driver_str, "%s;%s;%s;%.3f;%hu;%.3f", name, gender, age, average_rating, total_rides, total_earned);

    free(name);
    free(gender);
    free(age);

    return driver_str;
}

char *get_q2(int index, Drivers_Catalog catalog)
{ // change function name
    Driver driver = g_ptr_array_index(catalog->drivers_array, index);
    bool account_status = get_driver_account_status(driver);

    if (!account_status) // change compare function
        return NULL;

    char *driver_id = get_driver_id(driver);
    char *name = get_driver_name(driver);
    double average_rating = get_driver_average_rating(driver);

    char *result = malloc(strlen(driver_id) + strlen(name) + 5 + 2 + 1);
    sprintf(result, "%s;%s;%0.3f", driver_id, name, average_rating);

    free(driver_id);
    free(name);

    return result;
}

void free_drivers_catalog(Drivers_Catalog catalog)
{
    g_ptr_array_free(catalog->drivers_array, TRUE);
    g_hash_table_destroy(catalog->drivers_ht);
    free(catalog);
}
