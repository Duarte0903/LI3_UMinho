#include <glib.h>
#include <stdio.h>
#include <stdarg.h>
#include "../includes/rides-catalog.h"
#include "../includes/drivers-catalog.h"
#include "../includes/users-catalog.h"
#include "../includes/ride.h"

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

double calculate_ride_cost_w_tip(char *car_class, unsigned short distance, float tip) {
    double result = tip;

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

    float ride_tip = get_ride_tip(ride);
    unsigned short ride_distance = get_ride_distance(ride);
    char *ride_car_class = get_ride_car_class(driver_id, drivers_catalog);
    float ride_cost_w_tip = calculate_ride_cost_w_tip(ride_car_class, ride_distance, ride_tip);
    // casting double to float is ok since we do not need much precision to calculate money
    unsigned short ride_user_score = get_ride_user_score(ride);
    unsigned short ride_driver_score = get_ride_driver_score(ride);
    unsigned short ride_date = get_ride_date(ride);
    free(ride_car_class);

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

void free_rides_catalog(Rides_Catalog catalog) {
    g_ptr_array_free(catalog->rides_array, TRUE);
    free(catalog);
}
 