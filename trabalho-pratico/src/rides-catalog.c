#include <glib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "../includes/rides-catalog.h"
#include "../includes/drivers-catalog.h"
#include "../includes/users-catalog.h"
#include "../includes/ride.h"
#include "../includes/utils.h"
#include "../includes/date.h"

#define N_USER_STATS 4
#define N_DRIVER_STATS 3

typedef struct rides_catalog {
    GPtrArray *rides_array;
} *Rides_Catalog;

void glib_wrapper_free_ride(gpointer ride) {
    free_ride(ride);
}

Rides_Catalog create_rides_catalog() {
    Rides_Catalog catalog = malloc(sizeof(struct rides_catalog));

    catalog->rides_array = g_ptr_array_new_with_free_func(glib_wrapper_free_ride);
    
    return catalog;
}

double calculate_ride_cost(char *car_class, unsigned short distance) {
    double result = 0.0;

    if (!strcmp(car_class, "basic"))
        result += 3.25 + 0.62 * distance;
    else if (!strcmp(car_class, "green"))
        result += 4 + 0.79 * distance;
    else
        result += 5.20 + 0.94 * distance;

    return result;
}

void **get_user_stats_from_ride(unsigned short ride_user_score, float ride_cost_w_tip, unsigned short ride_distance, unsigned short ride_date) {
    void **stats = malloc(N_USER_STATS * sizeof(void *));

    if (!stats) {
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

void **get_driver_stats_from_ride(unsigned short ride_driver_score, float ride_cost_w_tip, unsigned short ride_date) {
    void **stats = malloc(N_DRIVER_STATS * sizeof(void *));

    if (!stats) {
        perror("Error: Failed to allocate memory!\n");
        exit(EXIT_FAILURE);
    }

    unsigned short *score = malloc(sizeof(unsigned short));  // otimizar ? (não sei se dá)
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

void insert_ride_in_catalog(char **fields, va_list args) {
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

static gint compare_rides_by_date(gconstpointer r1, gconstpointer r2) {
    Ride ride1 = *(Ride *)r1;
    Ride ride2 = *(Ride *)r2;

    unsigned short date1 = get_ride_date(ride1);
    unsigned short date2 = get_ride_date(ride2);

    return (date1 < date2) ? -1 : (date1 > date2);
}

static gint compare_ride_date_with_date(gconstpointer r, gconstpointer d) {
    Ride ride = *(Ride *)r;
    unsigned short date = *(unsigned short *)d;

    unsigned short ride_date = get_ride_date(ride);

    return (ride_date < date) ? -1 : (ride_date > date);
}

void sort_rides_by_date(Rides_Catalog catalog) {
    g_ptr_array_sort(catalog->rides_array, compare_rides_by_date);
}

static gint compare_ride_by_city(gconstpointer r1, gconstpointer r2) {
    Ride ride1 = *(Ride *)r1;
    Ride ride2 = *(Ride *)r2;

    char *city1 = get_ride_city(ride1);
    char *city2 = get_ride_city(ride2);

    int result = strcmp(city1,city2);

    free(city1);
    free(city2);

    return result;
}

static gint compare_ride_city_w_city(gconstpointer r1, gconstpointer c) {
    Ride ride = *(Ride *)r1;
    char *city = *(char **)c;

    char *ride_city = get_ride_city(ride);

    int result = strcmp(ride_city,city);

    free(ride_city);

    return result;
}

void sort_rides_by_city(Rides_Catalog catalog) {
    g_ptr_array_sort(catalog->rides_array, compare_ride_by_city);
}

char *get_q4(char *city, Rides_Catalog catalog) {
    int first_elem = first_occurrence_ptr_array_bsearch(catalog->rides_array, compare_ride_city_w_city, &city, 0);

    if (first_elem == -1) return NULL;

    int last_elem = last_occurrence_ptr_array_bsearch(catalog->rides_array, compare_ride_city_w_city, &city, 0);
    int rides_in_city = last_elem - first_elem + 1;
    double average_ride_price_in_city = 0.0;

    for (int i = 0; i<rides_in_city; i++)
    {
        average_ride_price_in_city += (double)get_ride_cost(g_ptr_array_index(catalog->rides_array, first_elem+i));
    }

    average_ride_price_in_city /= rides_in_city;

    char *average_str = malloc(11);
    sprintf(average_str, "%.3f", average_ride_price_in_city);

    return average_str;
}

char *get_q5(unsigned short start_date, unsigned short end_date, Rides_Catalog catalog) {
    int first_elem = first_occurrence_ptr_array_bsearch(catalog->rides_array, compare_ride_date_with_date, &start_date, 1);
    int last_elem = last_occurrence_ptr_array_bsearch(catalog->rides_array, compare_ride_date_with_date, &end_date, 1);

    if (first_elem == -1 || last_elem == -1) // the dates do not exist in the array + out of bounds
        return NULL;

    int n = last_elem - first_elem + 1;
    double average_price = 0.0;

    for (int i = 0; i < n; i++) {
        average_price += (double)get_ride_cost(g_ptr_array_index(catalog->rides_array, first_elem + i));
    }
    average_price /= n;

    char *result = malloc(10 + 1); // 10 average price, 1 do \0
    sprintf(result, "%.3f", average_price);
        
    return result;
}

static gint compare_rides_q9(gconstpointer r1, gconstpointer r2) {
    int result;

    Ride ride1 = *(Ride *)r1;
    Ride ride2 = *(Ride *)r2;

    unsigned short distance1 = get_ride_distance(ride1);
    unsigned short date1 = get_ride_date(ride1);
    char *id1 = get_ride_id(ride1);
    float tip1 = get_ride_tip(ride1);

    unsigned short distance2 = get_ride_distance(ride2);
    unsigned short date2 = get_ride_date(ride2);
    char *id2 = get_ride_id(ride2);
    float tip2 = get_ride_tip(ride2);

    if ((distance1 < distance2) || (distance1 == distance2 && date1 < date2) || (tip1 == 0.0f && tip2 > 0.0f)) result = -1;

    if ((distance1 > distance2) || (distance1 == distance2 && date1 > date2) || (tip1 > 0.0f && tip2 == 0.0f)) result = 1;

    if (distance1 == distance2 && date1 == date2) result = strcmp(id1, id2);

    free(id1);
    free(id2);

    return result * (-1);
}

void sort_rides_by_distance(GPtrArray *arr) {
    g_ptr_array_sort(arr, compare_rides_q9);
}

int get_rides_first_date(Rides_Catalog catalog, unsigned short first_date) {
    return first_occurrence_ptr_array_bsearch(catalog->rides_array, compare_ride_date_with_date, &first_date, 1);
}

int get_rides_last_date(Rides_Catalog catalog, unsigned short last_date) {
    return last_occurrence_ptr_array_bsearch(catalog->rides_array, compare_ride_date_with_date, &last_date, 1);
}

static gint compare_ride_tip_w_zero(gconstpointer r1, gconstpointer z) {
    int result = 1;

    Ride ride = *(Ride *)r1;
    float zero = *(float *)z;

    float tip = get_ride_tip(ride);

    if (tip > zero) 
        result = 0;

    return result;
}

int get_last_ride_w_nonzero_tip(GPtrArray *arr) {
    float zero = 0.0f;
    int last_elem = last_occurrence_ptr_array_bsearch(arr, compare_ride_tip_w_zero, &zero, 0);
    return last_elem;
}

void copy_rides_to_results_array(Rides_Catalog catalog, int first_elem, int last_elem, GPtrArray *arr) {
    int n_rides = last_elem - first_elem + 1;

    for (int i = 0; i < n_rides; i++) {
        Ride ride = g_ptr_array_index(catalog->rides_array, i + first_elem);
        g_ptr_array_add(arr, ride);
    }
}

char *get_q9(int index, GPtrArray *arr) {
    Ride ride = g_ptr_array_index(arr, index);

    char *ride_id = get_ride_id(ride);
    char *ride_date = int_to_date(get_ride_date(ride)); 
    unsigned short ride_distance = get_ride_distance(ride);
    char *ride_city = get_ride_city(ride);
    double ride_tip = (double)get_ride_tip(ride);

    char *result = malloc(strlen(ride_id) + strlen(ride_city) + strlen(ride_date) + sizeof(unsigned short) + sizeof(float));
    sprintf(result, "%s;%s;%hu;%s;%.3f", ride_id, ride_date, ride_distance, ride_city, ride_tip);

    return result;
}

void free_rides_catalog(Rides_Catalog catalog) {
    g_ptr_array_free(catalog->rides_array, TRUE);
    free(catalog);
}
