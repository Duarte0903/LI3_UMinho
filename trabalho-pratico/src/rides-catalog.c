#include <glib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include "../includes/rides-catalog.h"
#include "../includes/drivers-catalog.h"
#include "../includes/users-catalog.h"
#include "../includes/ride.h"
#include "../includes/utils.h"
#include "../includes/date.h"

#define N_USER_STATS 4
#define N_DRIVER_STATS 3

typedef struct rides_catalog
{
    GPtrArray *rides_array;
} *Rides_Catalog;

typedef struct driver_average_by_city
{
    char *driver_id;
    double average_rating;
} *Driver_in_City;

Driver_in_City new_driver_in_city()
{
    Driver_in_City driver = malloc(sizeof(struct driver_average_by_city));
    driver->driver_id = NULL;
    return driver;
}

void glib_wrapper_free_ride(gpointer ride)
{
    free_ride(ride);
}

void free_driver_by_city(Driver_in_City driver)
{
    free(driver->driver_id);
    free(driver);
}

void glib_wrapper_free_driver_in_city(gpointer driver)
{
    free_driver_by_city(driver);
}

Rides_Catalog create_rides_catalog()
{
    Rides_Catalog catalog = malloc(sizeof(struct rides_catalog));

    catalog->rides_array = g_ptr_array_new_with_free_func(glib_wrapper_free_ride);

    return catalog;
}

int is_valid_ride(char **fields) {
    if (IS_EMPTY(fields[0]) || IS_EMPTY(fields[2]) || IS_EMPTY(fields[3]) || IS_EMPTY(fields[4]))
        return 0;

    int day, month, year;

    if (!(sscanf(fields[1], "%2d/%2d/%4d", &day, &month, &year) == 3 && is_valid_date(day, month, year)))
        return 0;

    if (IS_EMPTY(fields[5]) || !is_positive_integer(fields[5]))
        return 0;

    if (IS_EMPTY(fields[6]) || !is_positive_integer(fields[6]))
        return 0;

    if (IS_EMPTY(fields[7]) || !is_positive_integer(fields[7]))
        return 0;

    if (IS_EMPTY(fields[8]) || !is_non_negative_float(fields[8]))
        return 0;

    return 1;
}

double calculate_ride_cost(char *car_class, unsigned short distance) {
    double result = 0.0;

    if (!strcasecmp(car_class, "basic"))
        result += 3.25 + 0.62 * distance;
    else if (!strcasecmp(car_class, "green"))
        result += 4 + 0.79 * distance;
    else
        result += 5.20 + 0.94 * distance;

    return result;
}

void **get_user_stats_from_ride(unsigned short ride_user_score, float ride_cost_w_tip, unsigned short ride_distance, unsigned short ride_date)
{
    void **stats = malloc(N_USER_STATS * sizeof(void *));

    if (!stats)
    {
        perror("Error: Failed to allocate memory!\n");
        exit(EXIT_FAILURE);
    }

    unsigned short *score = malloc(sizeof(unsigned short)); // otimizar ? (não sei se dá)
    float *cost = malloc(sizeof(float));
    unsigned short *distance = malloc(sizeof(unsigned short));
    unsigned short *date = malloc(sizeof(unsigned short));

    *score = ride_user_score;
    *cost = ride_cost_w_tip;
    *distance = ride_distance;
    *date = ride_date;

    stats[0] = score;
    stats[1] = cost;
    stats[2] = distance;
    stats[3] = date;

    return stats;
}

void **get_driver_stats_from_ride(unsigned short ride_driver_score, float ride_cost_w_tip, unsigned short ride_date)
{
    void **stats = malloc(N_DRIVER_STATS * sizeof(void *));

    if (!stats)
    {
        perror("Error: Failed to allocate memory!\n");
        exit(EXIT_FAILURE);
    }

    unsigned short *score = malloc(sizeof(unsigned short)); // otimizar ? (não sei se dá)
    float *cost = malloc(sizeof(float));
    unsigned short *date = malloc(sizeof(unsigned short));

    *score = ride_driver_score;
    *cost = ride_cost_w_tip;
    *date = ride_date;

    stats[0] = score;
    stats[1] = cost;
    stats[2] = date;

    return stats;
}

void insert_ride_in_catalog(char **fields, va_list args)
{
    Rides_Catalog rides_catalog = va_arg(args, Rides_Catalog);
    Users_Catalog users_catalog = va_arg(args, Users_Catalog);
    Drivers_Catalog drivers_catalog = va_arg(args, Drivers_Catalog);

    Ride ride = create_ride(fields);
    g_ptr_array_add(rides_catalog->rides_array, ride);

    char *username = get_ride_user(ride);
    char *driver_id = get_ride_driver_id(ride);
    unsigned short ride_distance = get_ride_distance(ride);
    char *ride_car_class = get_ride_car_class(driver_id, drivers_catalog);
    float ride_cost = calculate_ride_cost(ride_car_class, ride_distance);
    // casting double to float is ok since we do not need much precision to calculate monetary values
    set_ride_cost(ride, ride_cost);
    free(ride_car_class);

    float ride_tip = get_ride_tip(ride);
    float ride_cost_w_tip = ride_cost + ride_tip;
    unsigned short ride_user_score = get_ride_user_score(ride);
    unsigned short ride_driver_score = get_ride_driver_score(ride);
    unsigned short ride_date = get_ride_date(ride);

    void **user_stats_fields = get_user_stats_from_ride(ride_user_score, ride_cost_w_tip, ride_distance, ride_date);
    update_user_stats(username, user_stats_fields, users_catalog);
    free(username);
    for (int i = 0; i < N_USER_STATS; i++)
        free(user_stats_fields[i]);
    free(user_stats_fields);

    void **driver_stats_fields = get_driver_stats_from_ride(ride_driver_score, ride_cost_w_tip, ride_date);
    update_driver_stats(driver_id, driver_stats_fields, drivers_catalog);
    free(driver_id);
    for (int i = 0; i < N_DRIVER_STATS; i++)
        free(driver_stats_fields[i]);
    free(driver_stats_fields);
}

static gint compare_rides_by_date(gconstpointer r1, gconstpointer r2)
{
    Ride ride1 = *(Ride *)r1;
    Ride ride2 = *(Ride *)r2;

    unsigned short date1 = get_ride_date(ride1);
    unsigned short date2 = get_ride_date(ride2);

    return (date1 < date2) ? -1 : (date1 > date2);
}

static gint compare_ride_date_with_date(gconstpointer r, gconstpointer d)
{
    Ride ride = *(Ride *)r;
    unsigned short date = *(unsigned short *)d;

    unsigned short ride_date = get_ride_date(ride);

    return (ride_date < date) ? -1 : (ride_date > date);
}

void sort_rides_by_date(Rides_Catalog catalog)
{
    g_ptr_array_sort(catalog->rides_array, compare_rides_by_date);
}

static gint compare_ride_by_city(gconstpointer r1, gconstpointer r2)
{
    Ride ride1 = *(Ride *)r1;
    Ride ride2 = *(Ride *)r2;

    char *city1 = get_ride_city(ride1);
    char *city2 = get_ride_city(ride2);

    int result = strcmp(city1, city2);

    free(city1);
    free(city2);

    return result;
}

static gint compare_ride_by_city_and_driver_id(gconstpointer r1, gconstpointer r2)
{
    Ride ride1 = *(Ride *)r1;
    Ride ride2 = *(Ride *)r2;

    char *city1 = get_ride_city(ride1);
    char *city2 = get_ride_city(ride2);

    int result = strcmp(city1, city2);

    if (result == 0)
    {
        char *id_1 = get_ride_driver_id(ride1);
        char *id_2 = get_ride_driver_id(ride2);

        result = strcmp(id_1, id_2);

        free(id_1);
        free(id_2);
    }

    free(city1);
    free(city2);

    return result;
}

static gint compare_driver_average_in_city(gconstpointer d1, gconstpointer d2)
{

    int result = 0;
    Driver_in_City driver_1 = *(Driver_in_City *)d1;
    Driver_in_City driver_2 = *(Driver_in_City *)d2;

    float diff = driver_1->average_rating - driver_2->average_rating;

    if (diff > 0)
    {
        result = -1;
    }
    else if (diff < 0)
    {
        result = 1;
    }
    else
    {
        result = -strcmp(driver_1->driver_id, driver_2->driver_id);
    }

    return result;
}

static gint compare_ride_city_w_city(gconstpointer r1, gconstpointer c)
{
    Ride ride = *(Ride *)r1;
    char *city = *(char **)c;

    char *ride_city = get_ride_city(ride);

    int result = strcmp(ride_city, city);

    free(ride_city);

    return result;
}

void sort_rides_by_city(Rides_Catalog catalog)
{
    g_ptr_array_sort(catalog->rides_array, compare_ride_by_city);
}

void sort_rides_by_city_and_driver_id(Rides_Catalog catalog)
{
    g_ptr_array_sort(catalog->rides_array, compare_ride_by_city_and_driver_id);
}

char *get_q4(char *city, Rides_Catalog catalog)
{
    int first_elem = first_occurrence_ptr_array_bsearch(catalog->rides_array, compare_ride_city_w_city, &city, 0);

    if (first_elem == -1) 
        return NULL;

    int last_elem = last_occurrence_ptr_array_bsearch(catalog->rides_array, compare_ride_city_w_city, &city, 0);
    int n_rides_in_city = last_elem - first_elem + 1;
    double average_price_in_city = 0.0;

    for (int i = 0; i < n_rides_in_city; i++) {
        average_price_in_city += (double)get_ride_cost(g_ptr_array_index(catalog->rides_array, first_elem+i));
    }
    average_price_in_city /= n_rides_in_city;

    char *result = malloc(11);
    sprintf(result, "%.3f", average_price_in_city);

    return result;
}

char *get_q5(unsigned short start_date, unsigned short end_date, Rides_Catalog catalog)
{
    int first_elem = first_occurrence_ptr_array_bsearch(catalog->rides_array, compare_ride_date_with_date, &start_date, 1);
    int last_elem = last_occurrence_ptr_array_bsearch(catalog->rides_array, compare_ride_date_with_date, &end_date, 1);

    if (first_elem == -1 || last_elem == -1) // the dates do not exist in the array + out of bounds
        return NULL;

    int n_rides = last_elem - first_elem + 1;
    double average_price = 0.0;

    for (int i = 0; i < n_rides; i++) {
        average_price += (double)get_ride_cost(g_ptr_array_index(catalog->rides_array, first_elem + i));
    }
    average_price /= n_rides;

    char *result = malloc(10 + 1); // 10 average price, 1 \0
    sprintf(result, "%.3f", average_price);

    return result;
}

char *get_q6(char *city, unsigned short start_date, unsigned short end_date, Rides_Catalog catalog)
{
    int first_elem = first_occurrence_ptr_array_bsearch(catalog->rides_array, compare_ride_date_with_date, &start_date, 1);
    int last_elem = last_occurrence_ptr_array_bsearch(catalog->rides_array, compare_ride_date_with_date, &end_date, 1);

    if (first_elem == -1 || last_elem == -1) // the dates do not exist in the array + out of bounds
        return NULL;
        
    int n_rides = last_elem - first_elem + 1; // number of rides in date interval
    int n_rides_in_city = 0;
    double average_distance_in_city = 0.0;

    for (int i = 0; i < n_rides; i++) {
        Ride ride = g_ptr_array_index(catalog->rides_array, first_elem + i);
        char *ride_city = get_ride_city(ride);

        if (strcmp(ride_city, city) == 0) {
            n_rides_in_city++;
            average_distance_in_city += (double)get_ride_distance(ride);
        }

        free(ride_city);
    }

    if (n_rides_in_city == 0)
        return NULL;

    average_distance_in_city /= n_rides_in_city;

    char *result = malloc(10 + 1);
    sprintf(result, "%.3f", average_distance_in_city);

    return result;
}

char *get_q7(unsigned short output_number, char *city, va_list args)
{

    (void)va_arg(args, Users_Catalog);
    Drivers_Catalog drivers_catalog = va_arg(args, Drivers_Catalog);
    Rides_Catalog rides_catalog = va_arg(args, Rides_Catalog);

    int i = 0;
    int first_elem = first_occurrence_ptr_array_bsearch(rides_catalog->rides_array, compare_ride_city_w_city, &city, 0);
    int last_elem = last_occurrence_ptr_array_bsearch(rides_catalog->rides_array, compare_ride_city_w_city, &city, 0);

    if (first_elem == -1 || last_elem == -1)
        return NULL;

    int n = last_elem - first_elem + 1;

    GPtrArray *results_array = g_ptr_array_new_with_free_func(glib_wrapper_free_driver_in_city);

    float average_rating = 0;
    int n_rides = 0;

    Ride ride = g_ptr_array_index(rides_catalog->rides_array, first_elem);
    char *driver_id = get_ride_driver_id(ride);
    unsigned short rating = get_ride_driver_score(ride);

    average_rating += rating;
    n_rides += 1;
    char *prev_id = strdup(driver_id);

    free(driver_id);

    for (i = 1; i < n; i++)
    {
        ride = g_ptr_array_index(rides_catalog->rides_array, first_elem + i);
        driver_id = get_ride_driver_id(ride);
        rating = get_ride_driver_score(ride);

        if (strcmp(driver_id, prev_id) == 0)
        {
            average_rating += rating;
            n_rides += 1;
        }
        else
        {
            bool account_status = get_driver_account_status_id(prev_id, drivers_catalog);
            if (account_status)
            {
                Driver_in_City driver = new_driver_in_city();
                driver->driver_id = strdup(prev_id);
                driver->average_rating = average_rating / n_rides;

                g_ptr_array_add(results_array, driver);
            }
            free(prev_id);

            average_rating = 0;
            n_rides = 1;
            average_rating += rating;
            prev_id = strdup(driver_id);

        }

        free(driver_id);
    }

    free(prev_id);

    g_ptr_array_sort(results_array, compare_driver_average_in_city);

    char *result = malloc(100 * output_number);
    result[0] = '\0';
    for (i = 0; i < output_number; i++)
    {
        char *line = malloc(100 * sizeof(char));
        Driver_in_City driver = g_ptr_array_index(results_array, i);
        char *name = get_driver_name_id(driver->driver_id, drivers_catalog);

        sprintf(line, "%s;%s;%.3f\n", driver->driver_id, name, driver->average_rating);
        strcat(result, line);
        free(line);
        free(name);
    }

    result[strcspn(result, "\0") - 1] = '\0';

    g_ptr_array_free(results_array, TRUE);

    return result;
}

void free_rides_catalog(Rides_Catalog catalog)
{
    g_ptr_array_free(catalog->rides_array, TRUE);
    free(catalog);
}
