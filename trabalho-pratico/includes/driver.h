#ifndef DRIVER_H
#define DRIVER_H

#include <stdbool.h>
#include "../includes/vp_array.h"

typedef struct driver *Driver;

Driver create_driver(char **fields);

unsigned int get_driver_id(Driver driver);

char *get_driver_name(Driver driver);

char get_driver_gender(Driver driver);

char *get_driver_age(Driver driver);

char *get_driver_car_class(Driver driver);

bool get_driver_account_status(Driver driver);

double get_driver_average_rating (Driver driver, int index);

unsigned short get_driver_total_rides (Driver driver);

double get_driver_total_earned_money(Driver driver);

unsigned short get_driver_latest_ride (Driver driver);

unsigned short get_driver_account_age(Driver driver);

unsigned short get_driver_account_creation(Driver driver);

void set_driver_stats(Driver driver, VPA *stats);

void free_driver(Driver driver);

#endif