#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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
    double tip;
    char *comment; /* informação inútil ? */
    // stats
    double cost;
} *Ride;

Ride init_ride()
{
    Ride ride = malloc(sizeof(struct ride));

    ride->id = NULL;
    ride->driver_id = NULL;
    ride->user = NULL;
    ride->city = NULL;
    ride->comment = NULL;
    ride->cost = 0.0;

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
    ride->tip = str_to_double(fields[8]);
    ride->comment = strdup(fields[9]);

    return ride;
}

char *get_ride_id(Ride ride)
{
    return strdup(ride->id);
}

unsigned short get_ride_date(Ride ride)
{
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

char *get_ride_city(Ride ride)
{
    return strdup(ride->city);
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

double get_ride_cost(Ride ride)
{
    return ride->cost;
}

void set_ride_cost(Ride ride, double cost)
{
    ride->cost = cost;
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
