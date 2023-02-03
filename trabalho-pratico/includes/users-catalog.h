#ifndef USERS_CATALOG_H
#define USERS_CATALOG_H

#include <stdbool.h>
#include "../includes/vp_array.h"

typedef struct users_catalog *Users_Catalog;

Users_Catalog create_users_catalog();

int is_valid_user(char **fields);

void insert_user_in_catalog(char **fields, va_list args);

void update_user_stats(char *username, VPA *stats, Users_Catalog catalog);

unsigned short get_user_account_age_w_username(char *username, Users_Catalog catalog);

char get_user_gender_username(char *username, Users_Catalog catalog);

char *get_user_name_username(char *username, Users_Catalog catalog);

bool get_user_account_status_username(char *username, Users_Catalog catalog);

char *get_user_q1(char *username, Users_Catalog catalog);

char *get_q3(int n_users, Users_Catalog catalog);

void free_users_catalog(Users_Catalog catalog);

#endif