#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <glib.h>
#include "../includes/driver.h"
#include "../includes/date.h"
#include "../includes/utils.h"
#include "../includes/vp_array.h"

#define REF_DAY "9/10/2022"

typedef struct driver {
    char *id;
    char *name;
    unsigned short birth_date;
    char *gender;
    char *car_class;
    unsigned short account_creation;
    bool account_status;

    struct driver_stats {
        GPtrArray *ratings;
        double total_earned_money;
        unsigned short latest_ride;
        unsigned short account_age;
    } stats;
} *Driver;

typedef struct driver_ratings {
    unsigned int rating;
    unsigned short rides;
} *Driver_Ratings;

Driver_Ratings create_driver_ratings() {
    Driver_Ratings ratings = malloc(sizeof(struct driver_ratings));
    ratings->rating = 0;
    ratings->rides = 0;
    return ratings;
}

void init_driver_ratings(GPtrArray *ratings_arr, int capacity) {
    for (int i = 0; i < capacity; i++) {
        Driver_Ratings rating = create_driver_ratings();
        g_ptr_array_add(ratings_arr, rating);
    }
}

Driver init_driver() {
    Driver driver = malloc(sizeof(struct driver));

    driver->id = NULL;
    driver->name = NULL;
    driver->gender = NULL;
    driver->car_class = NULL;
    driver->account_status = true;
    driver->stats.ratings = g_ptr_array_new_full(8, g_free);
    init_driver_ratings(driver->stats.ratings, 8);
    driver->stats.total_earned_money = 0.0;
    driver->stats.latest_ride = 0;
    driver->stats.account_age = 0;

    return driver;
}

Driver create_driver(char **fields) {
    Driver driver = init_driver();

    driver->id = strdup(fields[0]);
    driver->name = strdup(fields[1]);
    driver->birth_date = date_to_int(fields[2]);
    driver->gender = strdup(fields[3]);
    driver->car_class = lower_string(strdup(fields[4]));
    driver->account_creation = date_to_int(fields[7]);
    driver->stats.account_age = date_to_int(REF_DAY) - driver->account_creation;

    if (strcasecmp(fields[8], "active\n"))
        driver->account_status = false;

    return driver;
}

char *get_driver_id(Driver driver) {
    return strdup(driver->id);
}

char *get_driver_name(Driver driver) {
    return strdup(driver->name);
}

char *get_driver_age(Driver driver) {
    return get_age(driver->birth_date);
}

char *get_driver_gender(Driver driver) {
    return strdup(driver->gender);
}

char *get_driver_car_class(Driver driver) {
    return strdup(driver->car_class);
}

bool get_driver_account_status(Driver driver) {
    return driver->account_status;
}

double get_driver_average_rating(Driver driver, int index) {
    Driver_Ratings ratings = g_ptr_array_index(driver->stats.ratings, index);
    return (double)ratings->rating / ratings->rides;
}

unsigned short get_driver_total_rides(Driver driver) {
    Driver_Ratings total = g_ptr_array_index(driver->stats.ratings, 7);
    return total->rides;
}

double get_driver_total_earned_money(Driver driver) {
    return driver->stats.total_earned_money;
}

unsigned short get_driver_latest_ride(Driver driver) {
    return driver->stats.latest_ride;
}

unsigned short get_driver_account_creation(Driver driver) {
    return driver->account_creation;
}

unsigned short get_driver_account_age(Driver driver) {
    return driver->stats.account_age;
}

void set_driver_stats(Driver driver, VPA *stats) {
    unsigned short driver_score = *(unsigned short *)vpa_get(stats, 0);
    int index = *(int *)vpa_get(stats, 1);
    Driver_Ratings ratings = g_ptr_array_index(driver->stats.ratings, index);
    ratings->rating += driver_score;
    ratings->rides++;

    Driver_Ratings total = g_ptr_array_index(driver->stats.ratings, 7);
    total->rating += driver_score;
    total->rides++;

    double ride_cost_w_tip = *(double *)vpa_get(stats, 2);
    driver->stats.total_earned_money += ride_cost_w_tip;

    unsigned short ride_date = *(unsigned short *)vpa_get(stats, 3);
    if (driver->stats.latest_ride < ride_date)
        driver->stats.latest_ride = ride_date;
}

void free_driver(Driver driver) {
    free(driver->id);
    free(driver->name);
    free(driver->gender);
    free(driver->car_class);
    g_ptr_array_free(driver->stats.ratings, TRUE);
    free(driver);
}
