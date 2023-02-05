#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include <stdbool.h>

bool file_exists(const char *filename);

char *get_dataset();

void print_waiting_on_catalogs();

void launch_main_interface(int n_args, ...);

#endif