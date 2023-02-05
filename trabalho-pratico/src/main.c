#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include "../includes/parser.h"
#include "../includes/utils.h"
#include "../includes/users-catalog.h"
#include "../includes/drivers-catalog.h"
#include "../includes/rides-catalog.h"
#include "../includes/interface/interactive.h"

#define N_USER_FIELDS 7
#define N_DRIVER_FIELDS 9
#define N_RIDE_FIELDS 10
#define MAX_QUERY_ARGS 4
#define N_CATALOGS 3

int main(int argc, char **argv)
{
    if (argc == 3)
    {
        clock_t start_execution = clock();
        char *data_path = argv[1];
        char *query_path = argv[2];

        char *users_file = get_file(data_path, "/users.csv");
        char *drivers_file = get_file(data_path, "/drivers.csv");
        char *rides_file = get_file(data_path, "/rides.csv");

        Users_Catalog users_catalog = create_users_catalog();
        Drivers_Catalog drivers_catalog = create_drivers_catalog();
        Rides_Catalog rides_catalog = create_rides_catalog();

        clock_t start_users = clock();
        parse_file(users_file, N_USER_FIELDS, is_valid_user, insert_user_in_catalog, users_catalog);
        clock_t end_users = clock();
        double time_users = (double)(end_users - start_users) / CLOCKS_PER_SEC;
        printf("Users loaded in %f seconds\n", time_users);

        clock_t start_drivers = clock();
        parse_file(drivers_file, N_DRIVER_FIELDS, is_valid_driver, insert_driver_in_catalog, drivers_catalog);
        clock_t end_drivers = clock();
        double time_drivers = (double)(end_drivers - start_drivers) / CLOCKS_PER_SEC;
        printf("Drivers loaded in %f seconds\n", time_drivers);

        clock_t start_rides = clock();
        parse_file(rides_file, N_RIDE_FIELDS, is_valid_ride, insert_ride_in_catalog, rides_catalog, users_catalog, drivers_catalog);
        clock_t end_rides = clock();
        double time_rides = (double)(end_rides - start_rides) / CLOCKS_PER_SEC;
        printf("Rides loaded in %f seconds\n", time_rides);

        parse_query(query_path, MAX_QUERY_ARGS, users_catalog, drivers_catalog, rides_catalog);

        free_users_catalog(users_catalog);
        free_drivers_catalog(drivers_catalog);
        free_rides_catalog(rides_catalog);
        free(users_file);
        free(drivers_file);
        free(rides_file);
        clock_t end_execution = clock();
        double time_execution = (double)(end_execution - start_execution) / CLOCKS_PER_SEC;
        printf("Program executed in %f seconds\n", time_execution);
    }
    else
    {
        char *data_path = get_dataset();

        print_waiting_on_catalogs();

        char *users_file = get_file(data_path, "/users.csv");
        char *drivers_file = get_file(data_path, "/drivers.csv");
        char *rides_file = get_file(data_path, "/rides.csv");

        free(data_path);

        if (!file_exists(users_file) || !file_exists(drivers_file) || !file_exists(rides_file))
        {
            endwin();
            printf("Wrong path or incomplete folder.\n");
            return 1;
        }

        Users_Catalog users_catalog = create_users_catalog();
        Drivers_Catalog drivers_catalog = create_drivers_catalog();
        Rides_Catalog rides_catalog = create_rides_catalog();

        parse_file(users_file, N_USER_FIELDS, is_valid_user, insert_user_in_catalog, users_catalog);
        parse_file(drivers_file, N_DRIVER_FIELDS, is_valid_driver, insert_driver_in_catalog, drivers_catalog);
        parse_file(rides_file, N_RIDE_FIELDS, is_valid_ride, insert_ride_in_catalog, rides_catalog, users_catalog, drivers_catalog);

        launch_main_interface(N_CATALOGS, users_catalog, drivers_catalog, rides_catalog, users_file, drivers_file, rides_file);

        free_users_catalog(users_catalog);
        free_drivers_catalog(drivers_catalog);
        free_rides_catalog(rides_catalog);
        free(users_file);
        free(drivers_file);
        free(rides_file);
    }
    return 0;
}
