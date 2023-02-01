#ifndef INTERACTIVE_H
#define INTERACTIVE_H

char *choose_dataset();

void start_ncurses_print_waiting_on_catalogs();

void launch_main_interface(int n_args, ...);

#endif