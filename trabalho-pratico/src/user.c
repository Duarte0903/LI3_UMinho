#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../includes/user.h"
#include "../includes/date.h"
#include "../includes/utils.h"

typedef struct user {
    char *username;
    char *name;
    char *gender;
    unsigned short birth_date;
    unsigned short account_creation;
    char *pay_method;
    bool account_status;

    struct user_stats {
        float average_rating;
        unsigned short total_rides;
        float total_spent_money;
        unsigned short total_distance;
        unsigned short latest_ride;    
    } stats;
} *User;

User init_user() {
    User user = malloc(sizeof(struct user));

    user->username = NULL;
    user->name = NULL;
    user->gender = NULL;
    user->pay_method = NULL;
    user->account_status = true;
    user->stats.average_rating = 0.0f;
    user->stats.total_rides = 0;
    user->stats.total_spent_money = 0.0f;
    user->stats.total_distance = 0;
    user->stats.latest_ride = 0;

    return user;
}

User create_user(char **fields) {
    User user = init_user();

    user->username = strdup(fields[0]);
    user->name = strdup(fields[1]);
    user->gender = strdup(fields[2]);
    user->birth_date = date_to_int(fields[3]);
    user->account_creation = date_to_int(fields[4]);
    user->pay_method = strdup(fields[5]);

    if (strcmp(fields[6], "active\n")) /* return = 0 --> str1 == str2 */
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

float get_user_average_rating(User user) {
    return user->stats.average_rating;
}

unsigned short get_user_total_rides(User user) {
    return user->stats.total_rides;
}

float get_user_total_spent_money(User user) {
    return user->stats.total_spent_money;
}

unsigned short get_user_total_distance(User user) {
    return user->stats.total_distance;
}

unsigned short get_user_latest_ride(User user) {
    return user->stats.latest_ride;
}

void set_user_stats(User user, void **stats) {
    unsigned short user_score = *(unsigned short *)stats[0];
    float new_average_rating = (user->stats.average_rating * user->stats.total_rides + user_score) / (user->stats.total_rides + 1);
    user->stats.average_rating = new_average_rating;

    user->stats.total_rides++;

    float ride_cost_w_tip = *(float *)stats[1];
    user->stats.total_spent_money += ride_cost_w_tip;

    unsigned short ride_distance = *(unsigned short *)stats[2];
    user->stats.total_distance += ride_distance;

    unsigned short ride_date = *(unsigned short *)stats[3];
    if (user->stats.latest_ride < ride_date) 
        user->stats.latest_ride = ride_date;
}

void free_user(User user) {
    free(user->username);
    free(user->name);
    free(user->gender);
    free(user->pay_method);
    free(user);
}
