#ifndef DRIVER_H
#define DRIVER_H

#include <stdbool.h>

typedef struct driver *Driver;

Driver create_driver(char **fields);

char *get_driver_id(Driver driver);

void free_driver(Driver driver);

char *get_driver_name(Driver driver);

char *get_driver_gender(Driver driver);

char *get_driver_age(Driver driver);

char *get_driver_car_class(Driver driver);

unsigned short get_driver_total_rides (Driver driver);

double get_driver_average_rating (Driver driver);

unsigned short get_driver_latest_ride (Driver driver);

void set_driver_latest_ride (Driver driver, unsigned short new_latest_ride);

void set_driver_average_rating (Driver driver, unsigned short driver_score);

void set_driver_total_rides (Driver driver);

bool get_driver_account_status(Driver driver);

void print_driver(Driver driver);

#endif