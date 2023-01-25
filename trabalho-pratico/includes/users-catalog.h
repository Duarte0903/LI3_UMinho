#ifndef USERS_CATALOG_H
#define USERS_CATALOG_H

typedef struct users_catalog *Users_Catalog;

Users_Catalog create_users_catalog();

int is_valid_user(char **fields);

void insert_user_in_catalog(char **fields, va_list args);

void update_user_stats(char *username, void **stats, Users_Catalog catalog);

void sort_users_by_distance(Users_Catalog catalog);

char *get_user_q1(char *username, Users_Catalog catalog);

char *get_q3(int index, Users_Catalog catalog);

void free_users_catalog(Users_Catalog catalog);

#endif