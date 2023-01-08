#ifndef DRIVER_H
#define DRIVER_H

#include <stdbool.h>

typedef struct driver *Driver;

Driver create_driver(char **fields);

char *get_driver_id(Driver driver);

char *get_driver_name(Driver driver);

char *get_driver_gender(Driver driver);

char *get_driver_age(Driver driver);

char *get_driver_car_class(Driver driver);

bool get_driver_account_status(Driver driver);

float get_driver_average_rating (Driver driver);

unsigned short get_driver_total_rides (Driver driver);

float get_driver_total_earned_money(Driver driver);

unsigned short get_driver_latest_ride (Driver driver);

void set_driver_stats(Driver driver, void **stats);

void free_driver(Driver driver);

#endif