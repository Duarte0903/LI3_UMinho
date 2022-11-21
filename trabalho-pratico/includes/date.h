#ifndef DATE_H
#define DATE_H

typedef struct date Date;

unsigned short date_to_int(char *date);

char *int_to_date(unsigned short date);

#endif