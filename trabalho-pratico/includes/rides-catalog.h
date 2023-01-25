#ifndef RIDES_CATALOG_H
#define RIDES_CATALOG_H

typedef struct rides_catalog *Rides_Catalog;

Rides_Catalog create_rides_catalog();

int is_valid_ride(char **fields);

void insert_ride_in_catalog(char **fields, va_list args);

void sort_rides_by_date(Rides_Catalog catalog);

void sort_rides_by_city(Rides_Catalog catalog);

void sort_rides_by_city_and_driver_id(Rides_Catalog catalog);

char *get_q4(char *city, Rides_Catalog catalog);

char *get_q5(unsigned short start_date, unsigned short end_date, Rides_Catalog catalog);

char *get_q6(char *city, unsigned short start_date, unsigned short end_date, Rides_Catalog catalog);

char *get_q7(unsigned short output_number, char *city, va_list args);

void sort_rides_by_distance(GPtrArray *arr);

int get_rides_first_date(Rides_Catalog catalog, unsigned short first_date);

int get_rides_last_date(Rides_Catalog catalog, unsigned short last_date);

int get_last_ride_w_nonzero_tip(GPtrArray *arr);

void copy_rides_to_results_array(Rides_Catalog catalog, int first_elem, int last_elem, GPtrArray *arr);

char *get_q9(int index, GPtrArray *arr); 

void free_rides_catalog(Rides_Catalog catalog);

#endif