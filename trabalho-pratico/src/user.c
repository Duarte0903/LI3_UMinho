#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include "../includes/user.h"
#include "../includes/date.h"
#include "../includes/utils.h"
#include "../includes/vp_array.h"

#define REF_DAY "9/10/2022"

typedef struct user {
    char *username;
    char *name;
    char *gender;
    unsigned short birth_date;
    unsigned short account_creation;
    bool account_status;

    struct user_stats {
        unsigned int total_rating;
        unsigned short total_rides;
        double total_spent_money;
        unsigned short total_distance;
        unsigned short latest_ride;
        unsigned short account_age;
    } stats;
} *User;

User init_user() {
    User user = malloc(sizeof(struct user));

    user->username = NULL;
    user->name = NULL;
    user->gender = NULL;
    user->account_status = true;
    user->stats.total_rating = 0;
    user->stats.total_rides = 0;
    user->stats.total_spent_money = 0.0;
    user->stats.total_distance = 0;
    user->stats.latest_ride = 0;
    user->stats.account_age = 0;

    return user;
}

User create_user(char **fields) {
    User user = init_user();

    user->username = strdup(fields[0]);
    user->name = strdup(fields[1]);
    user->gender = strdup(fields[2]);
    user->birth_date = date_to_int(fields[3]);
    user->account_creation = date_to_int(fields[4]);
    user->stats.account_age = date_to_int(REF_DAY) - user->account_creation;

    if (strcasecmp(fields[6], "active\n")) /* return = 0 --> str1 == str2 */
        user->account_status = false;

    return user;
}

char *get_user_username(User user) {
    return strdup(user->username);
}

char *get_user_name(User user) {
    return strdup(user->name);
}

char *get_user_gender(User user) {
    return strdup(user->gender);
}

char *get_user_age(User user) {
    return get_age(user->birth_date);
}

bool get_user_account_status(User user) {
    return user->account_status;
}

double get_user_average_rating(User user) {
    return (double)user->stats.total_rating / user->stats.total_rides;
}

unsigned short get_user_total_rides(User user) {
    return user->stats.total_rides;
}

double get_user_total_spent_money(User user) {
    return user->stats.total_spent_money;
}

unsigned short get_user_total_distance(User user) {
    return user->stats.total_distance;
}

unsigned short get_user_latest_ride(User user) {
    return user->stats.latest_ride;
}

unsigned short get_user_account_age(User user) {
    return user->stats.account_age;
}

void set_user_stats(User user, VPA *stats) {
    unsigned short user_score = *(unsigned short *)vpa_get(stats, 0);
    user->stats.total_rating += user_score;

    user->stats.total_rides++;

    double ride_cost_w_tip = *(double *)vpa_get(stats, 1);
    user->stats.total_spent_money += ride_cost_w_tip;

    unsigned short ride_distance = *(unsigned short *)vpa_get(stats, 2);
    user->stats.total_distance += ride_distance;

    unsigned short ride_date = *(unsigned short *)vpa_get(stats, 3);
    if (user->stats.latest_ride < ride_date)
        user->stats.latest_ride = ride_date;
}

void free_user(User user) {
    free(user->username);
    free(user->name);
    free(user->gender);
    free(user);
}
