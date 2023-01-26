#ifndef RIDES_CATALOG_H
#define RIDES_CATALOG_H

typedef struct rides_catalog *Rides_Catalog;

Rides_Catalog create_rides_catalog();

void insert_ride_in_catalog(char **fields, va_list args);

void sort_rides_by_date(Rides_Catalog catalog);

void sort_rides_by_city(Rides_Catalog catalog);

void sort_rides_by_city_and_driver_id(Rides_Catalog catalog);

void sort_rides_by_account_age(Rides_Catalog catalog, gpointer array_with_catalog_pointers);

char *get_q4(char *city, Rides_Catalog catalog);

char *get_q5(unsigned short start_date, unsigned short end_date, Rides_Catalog catalog);

char *get_q6(char *city, unsigned short start_date, unsigned short end_date, Rides_Catalog catalog);

char *get_q7(unsigned short output_number, char *city, va_list args);

char *get_q8(char *gender, int minimum_age, Rides_Catalog rides_catalog, gpointer catalog_pointer_extra_data);

void free_rides_catalog(Rides_Catalog catalog);

#endif