#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../includes/driver.h"
#include "../includes/date.h"
#include "../includes/utils.h"

#define REF_DAY "9/10/2022"

typedef struct driver
{
    char *id;
    char *name;
    unsigned short birth_date;
    char *gender;
    char *car_class;
    char *license_plate;
    char *city;
    unsigned short account_creation;
    bool account_status;

    struct driver_stats
    {
        float average_rating;
        unsigned short total_rides;
        float total_earned_money;
        unsigned short latest_ride;
        unsigned short account_age;
    } stats;
} *Driver;

Driver init_driver()
{
    Driver driver = malloc(sizeof(struct driver));

    driver->id = NULL;
    driver->name = NULL;
    driver->gender = NULL;
    driver->car_class = NULL;
    driver->license_plate = NULL;
    driver->city = NULL;
    driver->account_status = true;
    driver->stats.average_rating = 0.0f;
    driver->stats.total_rides = 0;
    driver->stats.total_earned_money = 0.0f;
    driver->stats.latest_ride = 0;
    driver->stats.account_age = 0;

    return driver;
}

Driver create_driver(char **fields)
{
    Driver driver = init_driver();

    driver->id = strdup(fields[0]);
    driver->name = strdup(fields[1]);
    driver->birth_date = date_to_int(fields[2]);
    driver->gender = strdup(fields[3]);
    driver->car_class = strdup(fields[4]);
    driver->license_plate = strdup(fields[5]);
    driver->city = strdup(fields[6]);
    driver->account_creation = date_to_int(fields[7]);
    ;
    driver->stats.account_age = date_to_int(REF_DAY) - driver->account_creation;

    if (strcmp(fields[8], "active\n")) /* return = 0 --> str1 == str2 */
        driver->account_status = false;

    return driver;
}

char *get_driver_id(Driver driver)
{
    return strdup(driver->id);
}

char *get_driver_name(Driver driver)
{
    return strdup(driver->name);
}

char *get_driver_age(Driver driver)
{
    return get_age(driver->birth_date);
}

char *get_driver_gender(Driver driver)
{
    return strdup(driver->gender);
}

char *get_driver_car_class(Driver driver)
{
    return strdup(driver->car_class);
}

bool get_driver_account_status(Driver driver)
{
    return driver->account_status;
}

float get_driver_average_rating(Driver driver)
{
    return driver->stats.average_rating;
}

unsigned short get_driver_total_rides(Driver driver)
{
    return driver->stats.total_rides;
}

float get_driver_total_earned_money(Driver driver)
{
    return driver->stats.total_earned_money;
}

unsigned short get_driver_latest_ride(Driver driver)
{
    return driver->stats.latest_ride;
}

unsigned short get_driver_account_age(Driver driver)
{
    return driver->stats.account_age;
}

void set_driver_stats(Driver driver, void **stats)
{
    unsigned short driver_score = *(unsigned short *)stats[0];
    float new_average_rating = (driver->stats.average_rating * driver->stats.total_rides + driver_score) / (driver->stats.total_rides + 1);
    driver->stats.average_rating = new_average_rating;

    driver->stats.total_rides++;

    float ride_cost_w_tip = *(float *)stats[1];
    driver->stats.total_earned_money += ride_cost_w_tip;

    unsigned short ride_date = *(unsigned short *)stats[2];
    if (driver->stats.latest_ride < ride_date)
        driver->stats.latest_ride = ride_date;
}

void free_driver(Driver driver)
{
    free(driver->id);
    free(driver->name);
    free(driver->gender);
    free(driver->car_class);
    free(driver->license_plate);
    free(driver->city);
    free(driver);
}
