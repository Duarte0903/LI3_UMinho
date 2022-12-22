#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../includes/parser.h"
#include "../includes/utils.h"
#include "../includes/users-catalog.h"
#include "../includes/drivers-catalog.h"
#include "../includes/rides-catalog.h"

#define N_USER_FIELDS 7
#define N_DRIVER_FIELDS 9
#define N_RIDE_FIELDS 10
#define MAX_QUERY_ARGS 4    

int main(int argc, char **argv) {
    if(argc == 3) {
        char *data_path = argv[1];
        char *query_path = argv[2];

        char *users_file = get_file(data_path, "/users.csv");
        char *drivers_file = get_file(data_path, "/drivers.csv");        
        char *rides_file = get_file(data_path, "/rides.csv");

        Users_Catalog users_catalog = create_users_catalog();
        Drivers_Catalog drivers_catalog = create_drivers_catalog();
        Rides_Catalog rides_catalog = create_rides_catalog();

        parse_file(users_file, N_USER_FIELDS, insert_user_in_catalog, users_catalog);
        parse_file(drivers_file, N_DRIVER_FIELDS, insert_driver_in_catalog, drivers_catalog);
        parse_file(rides_file, N_RIDE_FIELDS, insert_ride_in_catalog, rides_catalog, users_catalog, drivers_catalog);

        parse_query(query_path, MAX_QUERY_ARGS, users_catalog, drivers_catalog, rides_catalog);

        free_users_catalog(users_catalog);
        free_drivers_catalog(drivers_catalog);
        free_rides_catalog(rides_catalog);
        free(users_file);
        free(drivers_file);
        free(rides_file);
    } else {
        printf("Wrong number of arguments!\n");
    }
    return 0;
}
