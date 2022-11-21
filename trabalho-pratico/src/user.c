#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../includes/catalog.h"
#include "../includes/user.h"
#include "../includes/date.h"
#include "../includes/utils.h"

typedef struct user
{
    char *username;
    char *name;
    char *gender;
    unsigned short birth_date;
    unsigned short account_creation;
    char *pay_method;
    bool account_status;
    unsigned short total_distance;
    unsigned short latest_ride;
} * User;

User init_user()
{
    User user = malloc(sizeof(struct user));

    user->username = NULL;
    user->name = NULL;
    user->gender = NULL;
    user->pay_method = NULL;
    user->account_status = true;

    return user;
}

User create_user(char **fields)
{
    User user = init_user();

    user->username = strdup(fields[0]);
    user->name = strdup(fields[1]);
    user->gender = strdup(fields[2]);
    user->birth_date = date_to_int(fields[3]);
    user->account_creation = date_to_int(fields[4]);
    user->pay_method = strdup(fields[5]);
    user->total_distance = 0;
    user->latest_ride = 0;

    if (strcmp(fields[6], "active\n")) /* return = 0 --> str1 == str2 */
        user->account_status = false;

    return user;
}

char *get_user_username(User user)
{
    return strdup(user->username);
}

char *get_user_name(User user)
{
    return strdup(user->name);
}

char *get_user_gender(User user)
{
    return strdup(user->gender);
}

char *get_user_age(User user)
{
    return get_age(user->birth_date);
}

bool get_user_account_status(User user)
{
    return user->account_status;
}

unsigned short get_user_total_distance(User user) {
    return user->total_distance;
}

unsigned short get_user_latest_ride(User user) {
    return user->latest_ride;
}

void set_user_total_distance(User user, unsigned short new_distance) {
    user->total_distance += new_distance;
}

void set_user_latest_ride(User user, unsigned short new_latest_ride) {
    if (user->latest_ride < new_latest_ride)
        user->latest_ride = new_latest_ride;
}

void free_user(User user)
{
    free(user->username);
    free(user->name);
    free(user->gender);
    free(user->pay_method);
    free(user);
}

// For debug purposes
void print_user(User user)
{
    char *birth_date = int_to_date(user->birth_date);
    char *account_creation = int_to_date(user->account_creation);
    char *latest_ride = int_to_date(user->latest_ride);
    unsigned short distance = user->total_distance;

    char *account_status;

    if (user->account_status)
        account_status = "active";
    else
        account_status = "inactive";

    printf("[%s, %s, %s, %s, %s, %s, %s, %hu, %s]\n", user->username, user->name, user->gender, birth_date, account_creation, user->pay_method, account_status, distance, latest_ride);
    free(birth_date);
    free(account_creation);
    free(latest_ride);
}