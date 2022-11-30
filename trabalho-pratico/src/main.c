#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include "../includes/parser.h"
#include "../includes/utils.h"
#include "../includes/catalog.h"

#define _XOPEN_SOURCE_EXTENDED

#define N_USER_FIELDS 7
#define N_DRIVER_FIELDS 9
#define N_RIDE_FIELDS 10
#define MAX_QUERY_ARGS 4

int main(int argc, char **argv)
{
    if (argc == 3)
    {
        char *data_path = argv[1];
        char *query_path = argv[2];

        char *users_file = get_file(data_path, "/users.csv");
        char *drivers_file = get_file(data_path, "/drivers.csv");
        char *rides_file = get_file(data_path, "/rides.csv");

        Catalog catalog = create_catalog();

        parse_file(users_file, N_USER_FIELDS, insert_user_in_catalog, catalog);
        parse_file(drivers_file, N_DRIVER_FIELDS, insert_driver_in_catalog, catalog);
        parse_file(rides_file, N_RIDE_FIELDS, insert_ride_in_catalog, catalog);

        // print_catalog(catalog);

        parse_query(query_path, MAX_QUERY_ARGS, catalog);

        free_catalog(catalog);
        free(users_file);
        free(drivers_file);
        free(rides_file);
    }
    else
    {
        printf("Wrong number of arguments!\n");
    }
    initscr();                 /* Start curses mode           */
    printw("Hello World !!!"); /* Print Hello World          */
    refresh();                 /* Print it on to the real screen */
    sleep(4);
    endwin();                  /* End curses mode          */
    return 0;
}
