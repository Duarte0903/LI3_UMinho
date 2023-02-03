#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../includes/ride.h"
#include "../includes/date.h"
#include "../includes/utils.h"
#include "../includes/city_hash.h"

typedef struct ride {
    unsigned int id;
    unsigned short date;
    unsigned int driver_id;
    char *user;
    unsigned short city;
    unsigned short distance;
    unsigned short score_user;
    unsigned short score_driver;
    double tip;
    double cost;
} *Ride;

Ride init_ride() {
    Ride ride = malloc(sizeof(struct ride));

    ride->user = NULL;
    ride->cost = 0.0;

    return ride;
}

Ride create_ride(char **fields) {
    Ride ride = init_ride();

    ride->id = str_to_int(fields[0]);
    ride->date = date_to_int(fields[1]);
    ride->driver_id = str_to_int(fields[2]);
    ride->user = strdup(fields[3]);
    ride->city = get_city_index(fields[4]);
    ride->distance = str_to_int(fields[5]);
    ride->score_user = str_to_int(fields[6]);
    ride->score_driver = str_to_int(fields[7]);
    ride->tip = str_to_double(fields[8]);

    return ride;
}

unsigned int get_ride_id(Ride ride) {
    return ride->id;
}

unsigned short get_ride_date(Ride ride) {
    return ride->date;
}

unsigned int get_ride_driver_id(Ride ride) {
    return ride->driver_id;
}

char *get_ride_user(Ride ride) {
    return strdup(ride->user);
}

unsigned short get_ride_city(Ride ride) {
    return ride->city;
}

unsigned short get_ride_user_score(Ride ride) {
    return ride->score_user;
}

unsigned short get_ride_driver_score(Ride ride) {
    return ride->score_driver;
}

unsigned short get_ride_distance(Ride ride) {
    return ride->distance;
}

double get_ride_tip(Ride ride) {
    return ride->tip;
}

double get_ride_cost(Ride ride) {
    return ride->cost;
}

void set_ride_cost(Ride ride, double cost) {
    ride->cost = cost;
}

void free_ride(Ride ride) {
    free(ride->user);
    free(ride);
}
