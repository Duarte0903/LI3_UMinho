#ifndef CATALOG_H
#define CATALOG_H

typedef struct catalog *Catalog;

Catalog create_catalog();

void free_catalog(Catalog catalog);

void insert_user_in_catalog(char **fields, Catalog catalog);

void insert_driver_in_catalog(char **fields, Catalog catalog);

void insert_ride_in_catalog(char **fields, Catalog catalog);

void sort_q3(Catalog catalog);

void sort_q2 (Catalog catalog);

char *get_q2(int index, Catalog catalog);

char *get_q3(int index, Catalog catalog);

char *get_driver_q1(char *id, Catalog catalog);

char *get_user_q1(char *username, Catalog catalog);

void print_catalog(Catalog catalog);

#endif