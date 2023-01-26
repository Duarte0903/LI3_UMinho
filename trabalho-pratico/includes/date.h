#ifndef DATE_H
#define DATE_H

typedef struct date Date;

int is_valid_date(int day, int month, int year);

unsigned short date_to_int(char *date);

char *int_to_date(unsigned short date);

#endif