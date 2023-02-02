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

typedef struct drivers_catalog {
    GPtrArray *drivers_array;
    GHashTable *drivers_ht;
    enum sort_mode {
        UNSORTED,
        AVERAGE_RATING
    } sort_mode;
} *Drivers_Catalog;

void glib_wrapper_free_driver(gpointer driver) {
    free_driver(driver);
}

Drivers_Catalog create_drivers_catalog() {
    Drivers_Catalog catalog = malloc(sizeof(struct drivers_catalog));

    catalog->drivers_array = g_ptr_array_new(); // update to ptr array full?
    catalog->drivers_ht = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, glib_wrapper_free_driver);
    catalog->sort_mode = UNSORTED;

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

void update_driver_stats(char *driver_id, void **stats, Drivers_Catalog catalog) { // Improve function after input validation
    Driver driver = g_hash_table_lookup(catalog->drivers_ht, driver_id);
    set_driver_stats(driver, stats);
}

char *get_ride_car_class(char *driver_id, Drivers_Catalog catalog) {
    Driver driver = g_hash_table_lookup(catalog->drivers_ht, driver_id);
    return get_driver_car_class(driver);
}

char *get_driver_name_id(char *driver_id, Drivers_Catalog catalog) {
    Driver driver = g_hash_table_lookup(catalog->drivers_ht, driver_id);
    return get_driver_name(driver);
}

char *get_driver_gender_id(char *id, Drivers_Catalog catalog) {
    Driver driver = g_hash_table_lookup(catalog->drivers_ht, id);
    return get_driver_gender(driver);
}

bool get_driver_account_status_id(char *driver_id, Drivers_Catalog catalog) {
    Driver driver = g_hash_table_lookup(catalog->drivers_ht, driver_id);
    return get_driver_account_status(driver);
}

unsigned short get_driver_account_age_w_id(char *driver_id, Drivers_Catalog catalog) {
    Driver driver = g_hash_table_lookup(catalog->drivers_ht, driver_id);
    return get_driver_account_age(driver);
}

static gint compare_drivers_by_average_rating(gconstpointer d1, gconstpointer d2) {
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

    double average_rating1 = get_driver_average_rating(driver1);
    unsigned short date1 = get_driver_latest_ride(driver1);

    double average_rating2 = get_driver_average_rating(driver2);
    unsigned short date2 = get_driver_latest_ride(driver2);

    int nearly_equal = nearly_equal_fp_numbers(average_rating1, average_rating2, 0.00001f);
    int result;
    // Descending order: average rating, date
    // Ascending order: driver_id
    if ((!nearly_equal && average_rating1 < average_rating2) || (nearly_equal && date1 < date2))
        result = 1;
    else if ((!nearly_equal && average_rating1 > average_rating2) || (nearly_equal && date1 > date2))
        result = -1;
    else {
        char *driver_id1 = get_driver_id(driver1);
        char *driver_id2 = get_driver_id(driver2);
        result = strcmp(driver_id1, driver_id2);
        free(driver_id1);
        free(driver_id2);
    }

    return result;
}

void sort_drivers_by_average_rating(Drivers_Catalog catalog) {
    if (catalog->sort_mode != AVERAGE_RATING) {
        g_ptr_array_sort(catalog->drivers_array, compare_drivers_by_average_rating);
        catalog->sort_mode = AVERAGE_RATING;
    }
}

char *get_driver_q1(char *id, Drivers_Catalog catalog) { // change function and output variable name
    Driver driver = g_hash_table_lookup(catalog->drivers_ht, id);

    if (!driver || !get_driver_account_status(driver))
        return NULL;

    char *name = get_driver_name(driver);
    char *gender = get_driver_gender(driver);
    char *age = get_driver_age(driver);
    double average_rating = get_driver_average_rating(driver);
    unsigned short total_rides = get_driver_total_rides(driver);
    double total_earned = get_driver_total_earned_money(driver);

    char *driver_str = malloc(strlen(name) + strlen(gender) + strlen(age) + 5 + 10 + 10 + 5 + 2); // 5 de rating, 10 de total_rides, 10 de money, 5 dos ;, 2 do \n e \0
    sprintf(driver_str, "%s;%s;%s;%.3f;%hu;%.3f\n", name, gender, age, average_rating, total_rides, total_earned);

    free(name);
    free(gender);
    free(age);

    return driver_str;
}

char *get_q2(int n_drivers, Drivers_Catalog catalog) {
    if (n_drivers > (int)catalog->drivers_array->len)
        n_drivers = catalog->drivers_array->len;

    char *result = NULL;
    size_t result_size = 0;
    FILE *stream = open_memstream(&result, &result_size);
    char *driver_id = NULL, *name = NULL;
    double average_rating;

    for (int i = 0; i < n_drivers; i++) {
        Driver driver = g_ptr_array_index(catalog->drivers_array, i);
        driver_id = get_driver_id(driver);
        name = get_driver_name(driver);
        average_rating = get_driver_average_rating(driver);
        fprintf(stream, "%s;%s;%0.3f\n", driver_id, name, average_rating);
        free(driver_id);
        free(name);
    }

    fclose(stream);

    return result;
}

void free_drivers_catalog(Drivers_Catalog catalog) {
    g_ptr_array_free(catalog->drivers_array, TRUE);
    g_hash_table_destroy(catalog->drivers_ht);
    free(catalog);
}
