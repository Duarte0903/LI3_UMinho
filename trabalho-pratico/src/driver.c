#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../includes/catalog.h"
#include "../includes/driver.h"
#include "../includes/date.h"
#include "../includes/utils.h"

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
    float average_rating;
    unsigned short total_rides;
    unsigned short latest_ride;
    bool account_status;
} * Driver;

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
    driver->average_rating = 0.0;
    driver->total_rides = 0;
    driver->latest_ride = 0;

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

char *get_driver_gender(Driver driver)
{
    return strdup(driver->gender);
}

char *get_driver_age(Driver driver)
{
    return get_age(driver->birth_date);
}

char *get_driver_car_class(Driver driver)
{
    return strdup(driver->car_class);
}

bool get_driver_account_status(Driver driver)
{
    return driver->account_status;
}

unsigned short get_driver_total_rides (Driver driver)
{
    return driver->total_rides;
}

float get_driver_average_rating (Driver driver)
{
    return driver->average_rating;
}

unsigned short get_driver_latest_ride (Driver driver)
{
    return driver->latest_ride;
}

void set_driver_latest_ride (Driver driver, unsigned short new_latest_ride)
{
    if (driver->latest_ride < new_latest_ride)
        driver->latest_ride = new_latest_ride;
}

void set_driver_average_rating (Driver driver, unsigned short driver_score)
{
    float new_driver_average_rating = (driver->average_rating * driver->total_rides + driver_score) / (driver->total_rides + 1);

    driver->average_rating = new_driver_average_rating;
}

void set_driver_total_rides (Driver driver)
{
    driver->total_rides++;
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

// For debug purposes
void print_driver(Driver driver)
{
    char *birth_date = int_to_date(driver->birth_date);
    char *account_creation = int_to_date(driver->account_creation);
    char *account_status;

    if (driver->account_status)
        account_status = "active";
    else
        account_status = "inactive";

    printf("[%s, %s, %s, %s, %s, %s, %s, %s, %s]\n", driver->id, driver->name, birth_date, driver->gender, driver->car_class, driver->license_plate, driver->city, account_creation, account_status);
    free(birth_date);
    free(account_creation);
}
