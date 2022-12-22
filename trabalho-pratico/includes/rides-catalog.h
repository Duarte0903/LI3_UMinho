#ifndef RIDES_CATALOG_H
#define RIDES_CATALOG_H

typedef struct rides_catalog *Rides_Catalog;

Rides_Catalog create_rides_catalog();

void insert_ride_in_catalog(char **fields, va_list args);

void free_rides_catalog(Rides_Catalog catalog);

#endif