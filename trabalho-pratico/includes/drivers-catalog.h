#ifndef DRIVERS_CATALOG_H
#define DRIVERS_CATALOG_H

#include <stdbool.h>

typedef struct drivers_catalog *Drivers_Catalog;

Drivers_Catalog create_drivers_catalog();

void insert_driver_in_catalog(char **fields, va_list args);

void update_driver_stats(char *driver_id, void **stats, Drivers_Catalog catalog);

char *get_ride_car_class(char *driver_id, Drivers_Catalog catalog);

char *get_driver_name_id(char *driver_id, Drivers_Catalog catalog);

char *get_driver_gender_id(char *id, Drivers_Catalog catalog);

bool get_driver_account_status_id(char *driver_id, Drivers_Catalog catalog);

unsigned short get_driver_account_age_w_id(char *driver_id, Drivers_Catalog catalog);

void sort_drivers_by_average_rating(Drivers_Catalog catalog);

char *get_driver_q1(char *id, Drivers_Catalog catalog);

char *get_q2(int index, Drivers_Catalog catalog);

void free_drivers_catalog(Drivers_Catalog catalog);

#endif