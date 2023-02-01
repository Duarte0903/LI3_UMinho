#ifndef RIDE_H
#define RIDE_H

typedef struct ride *Ride;

Ride create_ride(char **fields);

char *get_ride_id(Ride ride);

unsigned short get_ride_date(Ride ride);

char *get_ride_driver_id(Ride ride);

char *get_ride_user(Ride ride);

char *get_ride_city(Ride ride);

unsigned short get_ride_user_score(Ride ride);

unsigned short get_ride_driver_score(Ride ride);

unsigned short get_ride_distance(Ride ride);

double get_ride_tip(Ride ride);

double get_ride_cost(Ride ride);

void set_ride_cost(Ride ride, double cost);

void free_ride(Ride ride);

#endif