#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../includes/date.h"
#include "../includes/utils.h"

typedef struct date {
    int d, m, y;
} Date;

Date convert_string_to_date(char *date) {
    Date result;
    char *temp = strdup(date);
    char *save = temp;

    result.d = str_to_int(strsep(&temp, "/"));
    result.m = str_to_int(strsep(&temp, "/"));
    result.y = str_to_int(strsep(&temp, "\0"));

    free(save);
    return result;
}

char *convert_date_to_string(Date d) {
    char day[3];
    char month[3];
    char year[5];
    sprintf(day, "%d", d.d);
    sprintf(month, "%d", d.m);
    sprintf(year, "%d", d.y);

    size_t size = strlen(day) + strlen(month) + strlen(year) + 3;
    char *result = malloc(size);
    snprintf(result, size, "%s/%s/%s", day, month, year);

    return result;
}

int count_leap_years(Date d) {
    int years = d.y;
    // Check if the current year needs to be considered for the count of leap years or not
    if (d.m <= 2)
            years--;
    /*
    A year is a leap year only if it is either:
    --> multiple of 4 and not a multiple of 100
    --> multiple of 400
    */
    return (years / 4 - years / 100 + years / 400);
}

unsigned short date_to_int(char *date) {
    const int month_days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    Date d = convert_string_to_date(date);
    // Number of days before d1 (1/1/1900) that is our reference date (number 0)
    int n1 = 693961;

    // Count total number of days before d2
    // Initialize count using years and day
    int n2 = d.y * 365 + d.d;
    // Add days for months in given date
    for(int i = 0; i < d.m - 1; i++)
            n2 += month_days[i];
    // Since every leap year is of 366 days,
    // Add a day for every leap year
    n2 += count_leap_years(d);
    // Return difference between two counts
    return (n2 - n1);
}

char *int_to_date(unsigned short date) {
    Date d;
    char *result;
    //Number of days between 1 January 1900 and the date we want to find
    int days_since_epoch = date;
    int z = days_since_epoch + 693901; //693901: days from 01-03-0000 to 01-01-1900
    /*
    The algorithm divides time into eras of 400 years each (1600-2000, etc) as the gregorian calendar
    repeats itself every 400 years. Therefore, the era can be factored out of the computation
    */
    int era = (z >= 0 ? z : z - 146096) / 146097; //The current era (currently equal to 5)
    //Days of current era (146097 is number of days in era)
    int doe = z - era * 146097;                                 // [0, 146096]
    //Year of current era (subtractions of doe/x is to remove leap days: there is one every 1460 days, every 3146096 days,
    //but no every 36524 days(multiples of 100 and not 400 are not leap years))
    int yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;  // [0, 399]
    //Get the day of the year (account for leap years): Note it's day of the year counting from March 1st
    int doy = doe - (365 * yoe + yoe / 4 - yoe / 100);                // [0, 365]
    //Get the month (counting from March)
    int mp = (5 * doy + 2) / 153;                                     // [0, 11]
    //Getting the correct day of the month
    d.d = doy - (153 * mp + 2) / 5 + 1;                          // [1, 31]
    //Restoring the correct month (shifting the start back to January)
    d.m = mp + (mp < 10 ? 3 : -9);                             // [1, 12]
    //Restoring the current year
    d.y = yoe + era * 400 + (d.m > 2 ? 0 : 1);

    result = convert_date_to_string(d);

    return result;
}
