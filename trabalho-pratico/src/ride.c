#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../includes/catalog.h"
#include "../includes/ride.h"
#include "../includes/date.h"
#include "../includes/utils.h"

typedef struct ride
{
    char *id;
    unsigned short date;
    char *driver_id;
    char *user;
    char *city;
    unsigned short distance;
    unsigned short score_user;
    unsigned short score_driver;
    float tip;
    char *comment; /* informação inútil ? */
} * Ride;

Ride init_ride()
{
    Ride ride = malloc(sizeof(struct ride));

    ride->id = NULL;
    ride->driver_id = NULL;
    ride->user = NULL;
    ride->city = NULL;
    ride->comment = NULL;

    return ride;
}

Ride create_ride(char **fields)
{
    Ride ride = init_ride();

    ride->id = strdup(fields[0]);
    ride->date = date_to_int(fields[1]);
    ride->driver_id = strdup(fields[2]);
    ride->user = strdup(fields[3]);
    ride->city = strdup(fields[4]);
    ride->distance = (unsigned short)str_to_int(fields[5]);
    ride->score_user = (unsigned short)str_to_int(fields[6]);
    ride->score_driver = (unsigned short)str_to_int(fields[7]);
    ride->tip = str_to_float(fields[8]);
    ride->comment = strdup(fields[9]);

    return ride;
}

unsigned short get_ride_date(Ride ride) {
    return ride->date;
}

char *get_ride_driver_id(Ride ride)
{
    return strdup(ride->driver_id);
}

char *get_ride_user(Ride ride)
{
    return strdup(ride->user);
}

unsigned short get_ride_user_score(Ride ride)
{
    return ride->score_user;
}

unsigned short get_ride_driver_score(Ride ride)
{
    return ride->score_driver;
}

unsigned short get_ride_distance(Ride ride)
{
    return ride->distance;
}

double get_ride_tip(Ride ride)
{
    return ride->tip;
}

void free_ride(Ride ride)
{
    free(ride->id);
    free(ride->driver_id);
    free(ride->user);
    free(ride->city);
    free(ride->comment);
    free(ride);
}

unsigned short get_ride_score_driver (Ride ride)
{
    return ride->score_driver;
}

// For debug purposes
void print_ride(Ride ride)
{
    char *date = int_to_date(ride->date);
    char distance[5];
    char score_user[2];
    char score_driver[2];
    char tip[10];
    snprintf(distance, 5, "%hu", ride->distance);
    snprintf(score_user, 2, "%hu", ride->score_user);
    snprintf(score_driver, 2, "%hu", ride->score_driver);
    snprintf(tip, 10, "%.3f", (double)ride->tip); // fix this

    printf("[%s, %s, %s, %s, %s, %s, %s, %s, %s, %s]\n", ride->id, date, ride->driver_id, ride->user, ride->city, distance, score_user, score_driver, tip, ride->comment);
    free(date);
}
