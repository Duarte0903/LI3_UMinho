#ifndef RIDE_H
#define RIDE_H

typedef struct ride *Ride;

Ride create_ride(char **fields);

void free_ride(Ride ride);

unsigned short get_ride_score_driver (Ride ride);

unsigned short get_ride_date(Ride ride);

char *get_ride_driver_id(Ride ride);

char *get_ride_user(Ride ride);

unsigned short get_ride_user_score(Ride ride);

unsigned short get_ride_driver_score(Ride ride);

unsigned short get_ride_distance(Ride ride);

float get_ride_tip(Ride ride);

void print_ride(Ride ride);

#endif