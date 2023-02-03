#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>
#include "../includes/users-catalog.h"
#include "../includes/user.h"
#include "../includes/utils.h"
#include "../includes/date.h"
#include "../includes/vp_array.h"

typedef struct users_catalog {
    GPtrArray *users_array;
    GHashTable *users_ht;
    bool is_sorted;
} *Users_Catalog;

void glib_wrapper_free_user(gpointer user) {
    free_user(user);
}

Users_Catalog create_users_catalog() {
    Users_Catalog catalog = malloc(sizeof(struct users_catalog));

    catalog->users_array = g_ptr_array_new_full(100000, NULL);
    catalog->users_ht = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, glib_wrapper_free_user);
    catalog->is_sorted = false;

    return catalog;
}

int is_valid_user(char **fields) {
    if (IS_EMPTY(fields[0]) || IS_EMPTY(fields[1]) || IS_EMPTY(fields[2]) || IS_EMPTY(fields[5]))
        return 0;

    int day, month, year;

    if (!(sscanf(fields[3], "%2d/%2d/%4d", &day, &month, &year) == 3 && is_valid_date(day, month, year)))
        return 0;

    if (!(sscanf(fields[4], "%2d/%2d/%4d", &day, &month, &year) == 3 && is_valid_date(day, month, year)))
        return 0;

    if (IS_EMPTY(fields[6]) || (strcasecmp(fields[6], "active\n") != 0 && strcasecmp(fields[6], "inactive\n") != 0))
        return 0;

    return 1;
}

void insert_user_in_catalog(char **fields, va_list args) {
    Users_Catalog catalog = va_arg(args, Users_Catalog);
    User user = create_user(fields);
    char *key = get_user_username(user);

    g_hash_table_insert(catalog->users_ht, key, user);
    g_ptr_array_add(catalog->users_array, user);
}

void update_user_stats(char *username, VPA *stats, Users_Catalog catalog) { // Improve function after input validation
    User user = g_hash_table_lookup(catalog->users_ht, username);
    set_user_stats(user, stats);
}

char get_user_gender_username(char *username, Users_Catalog catalog) {
    User user = g_hash_table_lookup(catalog->users_ht, username);
    return get_user_gender(user);
}

char *get_user_name_username(char *username, Users_Catalog catalog) {
    User user = g_hash_table_lookup(catalog->users_ht, username);
    return get_user_name(user);
}

bool get_user_account_status_username(char *username, Users_Catalog catalog) {
    User user = g_hash_table_lookup(catalog->users_ht, username);
    return get_user_account_status(user);
}

static gint compare_users_by_distance(gconstpointer u1, gconstpointer u2) {
    User user1 = *(User *)u1;
    User user2 = *(User *)u2;

    bool account_status1 = get_user_account_status(user1);
    bool account_status2 = get_user_account_status(user2);

    if (account_status1 && !account_status2)
        return -1;

    if (!account_status1 && account_status2)
        return 1;

    if (!account_status1 && !account_status2)
        return 0;

    unsigned short dist1 = get_user_total_distance(user1);
    unsigned short date1 = get_user_latest_ride(user1);

    unsigned short dist2 = get_user_total_distance(user2);
    unsigned short date2 = get_user_latest_ride(user2);

    int result;
    // Descending order: distance, date
    // Ascending order: username
    if (dist1 < dist2 || (dist1 == dist2 && date1 < date2))
        result = 1;
    else if (dist1 > dist2 || (dist1 == dist2 && date1 > date2))
        result = -1;
    else {
        char *username1 = get_user_username(user1);
        char *username2 = get_user_username(user2);
        result = strcmp(username1, username2);
        free(username1);
        free(username2);
    }

    return result;
}

void sort_users_by_distance(Users_Catalog catalog) {
    if (!catalog->is_sorted) {
        clock_t start_sort = clock();
        g_ptr_array_sort(catalog->users_array, compare_users_by_distance);
        clock_t end_sort = clock();
        double time_sort = (double)(end_sort - start_sort) / CLOCKS_PER_SEC;
        printf("Users sorted in %f seconds\n", time_sort);
        catalog->is_sorted = true;
    }
}

unsigned short get_user_account_age_w_username(char *username, Users_Catalog catalog) {
    User user = g_hash_table_lookup(catalog->users_ht, username);
    return get_user_account_age(user);
}

char *get_user_q1(char *username, Users_Catalog catalog) { // change function and output variable name
    User user = g_hash_table_lookup(catalog->users_ht, username);

    if (!user || !get_user_account_status(user))
        return NULL;

    char *name = get_user_name(user);
    char gender = get_user_gender(user);
    char *age = get_user_age(user);
    double average_rating = get_user_average_rating(user);
    unsigned short total_rides = get_user_total_rides(user);
    double total_spent = get_user_total_spent_money(user);

    char *user_str = malloc(strlen(name) + strlen(age) + 1 + 5 + 10 + 10 + 5 + 2); // 1 gender + 5 de rating, 10 de total_rides, 10 de money, 5 dos ;, 2 do \n e \0
    sprintf(user_str, "%s;%c;%s;%.3f;%hu;%.3f\n", name, gender, age, average_rating, total_rides, total_spent);

    free(name);
    free(age);

    return user_str;
}

char *get_q3(int n_users, Users_Catalog catalog) { // change function name
    sort_users_by_distance(catalog);
    
    if (n_users > (int)catalog->users_array->len)
        n_users = catalog->users_array->len;

    char *result = NULL;
    size_t result_size = 0;
    FILE *stream = open_memstream(&result, &result_size);
    char *username = NULL, *name = NULL;
    unsigned short total_distance;

    for (int i = 0; i < n_users; i++) {
        User user = g_ptr_array_index(catalog->users_array, i);
        username = get_user_username(user);
        name = get_user_name(user);
        total_distance = get_user_total_distance(user);
        fprintf(stream, "%s;%s;%hu\n", username, name, total_distance);
        free(username);
        free(name);
    }

    fclose(stream);

    return result;
}

void free_users_catalog(Users_Catalog catalog) {
    g_ptr_array_free(catalog->users_array, TRUE);
    g_hash_table_destroy(catalog->users_ht);
    free(catalog);
}
