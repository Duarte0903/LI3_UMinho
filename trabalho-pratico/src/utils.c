#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <limits.h>
#include "../includes/date.h"
#include "../includes/utils.h"

#define REF_DAY "9/10/2022"

char *get_age(unsigned short birth_date)
{
    char *age_str = malloc(4 * sizeof(char));
    unsigned short ref_day = date_to_int(REF_DAY);
    unsigned short age = (ref_day - birth_date) / 365.25;
    sprintf(age_str, "%hu", age);
    return age_str;
}

unsigned short get_age_no_ref(unsigned short birth_date)
{
    unsigned short age = birth_date / 365.25;
    return age;
}

char *get_file(char *path, const char *file)
{
    char *result = malloc(strlen(path) + strlen(file) + 1);
    strcpy(result, path);
    strcat(result, file);
    return result;
}

int is_positive_integer(char *str)
{
    char *end_ptr = str;
    errno = 0;
    unsigned long value = -1; // else return 0 instead?

    if (strchr(str, '-') == NULL)
        value = strtoul(str, &end_ptr, 10);

    /*
    --> check for minus sign in string: valid conversion (uses ULONG_MAX to find a value) but it needs to be excluded
    --> errno != 0 : conversion failed (EINVAL, ERANGE)
    --> str == end_ptr : conversion failed (no characters consumed)
    --> *end_ptr != 0 : conversion failed (trailing data)
    */

    return errno == 0 && str != end_ptr && *end_ptr == '\0' && value > 0;
}

int is_non_negative_float(char *str)
{
    char *end_ptr = str;
    errno = 0;

    double value = strtod(str, &end_ptr);

    /*
    --> errno != 0 : conversion failed (EINVAL, ERANGE)
    --> str == end_ptr : conversion failed (no characters consumed)
    --> *end_ptr != 0 : conversion failed (trailing data)
    --> isnan == 1 : valid conversion but it needs to be excluded
    --> isinf == 1 : valid conversion but it needs to be excluded
    */

    return errno == 0 && str != end_ptr && *end_ptr == '\0' && !isnan(value) && !isinf(value) && value >= 0;
}

int str_to_int(char *str)
{
    char *end; // cant be null
    errno = 0;
    int result = -1;                    // fix this
    long value = strtol(str, &end, 10); // strtoul

    if (errno == 0 && *end == '\0') // Boundary check not needed since we are going to do an input validation
        result = (int)value;

    return result;
}

float str_to_float(char *str)
{
    char *end; // cant be null
    errno = 0;
    float result = -1.0; // fix this
    float value = strtof(str, &end);

    if (errno == 0 && *end == '\0')
    {
        result = value;
    }

    return result;
}

/*
int round_float(float f, int precision) {
    float value = f * powf(10, precision);
    float truncated = floorf(value);
    float rounded = roundf(value);
    int result = rounded;

    if (value < 0)
        truncated = ceilf(value);

    if (fabs(value - truncated) == 0.5 && ((int)rounded % 2) != 0)
        result = rounded - 1;

    return result;
}

int floatcmp(float f1, float f2, int precision) {
    int rounded1 = round_float(f1, precision);
    int rounded2 = round_float(f2, precision);

    float val = 37.777779;                        // pow(10, (int)desiredPrecision)
    float rounded_down = floorf(val * 100) / 100; // Result: 37.77
    float nearest = roundf(val * 100) / 100;      // Result: 37.78
    float rounded_up = ceilf(val * 100) / 100;    // Result: 37.78

    return (rounded1 < rounded2) ? -1 : (rounded1 > rounded2);
}
*/

int nearly_equal_floats(float f1, float f2, float epsilon)
{ // check if the current value for epsilon is the best we can work with
    float absf1 = fabsf(f1);
    float absf2 = fabsf(f2);
    float diff = fabsf(f1 - f2); // https://floating-point-gui.de/

    if (f1 == f2) // shortcut, handles infinities
        return 1;
    else if (f1 == 0 || f2 == 0 || (absf1 + absf2 < __FLT_MIN__))
        // a or b is zero or both are extremely close to it
        // relative error is less meaningful here
        return diff < (epsilon * __FLT_MIN__);
    else // use relative error
        return diff / fminf((absf1 + absf2), __FLT_MAX__) < epsilon;
}

int first_occurrence_ptr_array_bsearch(GPtrArray *array, GCompareFunc compare_func, void *target, int search_bigger_nearest)
{
    if (compare_func == NULL || array == NULL)
    {
        perror("Error: Binary search failed!\n");
        exit(EXIT_FAILURE);
    }

    int left = 0, right = array->len - 1, result = -1;
    int middle, value;
    void *elem;

    while (left <= right)
    {
        middle = left + (right - left) / 2;
        elem = g_ptr_array_index(array, middle);
        value = compare_func(&elem, target);

        if (value > 0)
            right = middle - 1;
        else if (value < 0)
            left = middle + 1;
        else
        {
            result = middle;
            right = middle - 1;
        }
    }

    if (search_bigger_nearest == 1 && result == -1 && left < (int)array->len)
        result = right + 1;

    return result;
}

int last_occurrence_ptr_array_bsearch(GPtrArray *array, GCompareFunc compare_func, void *target, int search_smaller_nearest)
{
    if (compare_func == NULL || array == NULL)
    {
        perror("Error: Binary search failed!\n");
        exit(EXIT_FAILURE);
    }

    int left = 0, right = array->len - 1, result = -1;
    int middle, value;
    void *elem;

    while (left <= right)
    {
        middle = left + (right - left) / 2;
        elem = g_ptr_array_index(array, middle);
        value = compare_func(&elem, target);

        if (value > 0)
            right = middle - 1;
        else if (value < 0)
            left = middle + 1;
        else
        {
            result = middle;
            left = middle + 1;
        }
    }

    if (search_smaller_nearest == 1 && result == -1)
        result = left - 1;

    return result;
}

int first_occurrence_ptr_array_bsearch_with_data(GPtrArray *array, GCompareDataFunc compare_func, void *target, int search_bigger_nearest, gpointer extra_data)
{
    if (compare_func == NULL || array == NULL)
    {
        perror("Error: Binary search failed!\n");
        exit(EXIT_FAILURE);
    }

    int left = 0, right = array->len - 1, result = -1;
    int middle, value;
    void *elem;

    while (left <= right)
    {
        middle = left + (right - left) / 2;
        elem = g_ptr_array_index(array, middle);
        value = compare_func(&elem, target, extra_data);

        if (value > 0)
            right = middle - 1;
        else if (value < 0)
            left = middle + 1;
        else
        {
            result = middle;
            right = middle - 1;
        }
    }

    if (search_bigger_nearest == 1 && result == -1 && left < (int)array->len)
        result = right + 1;

    return result;
}

int last_occurrence_ptr_array_bsearch_with_data(GPtrArray *array, GCompareDataFunc compare_func, void *target, int search_smaller_nearest, gpointer extra_data)
{
    if (compare_func == NULL || array == NULL)
    {
        perror("Error: Binary search failed!\n");
        exit(EXIT_FAILURE);
    }

    int left = 0, right = array->len - 1, result = -1;
    int middle, value;
    void *elem;

    while (left <= right)
    {
        middle = left + (right - left) / 2;
        elem = g_ptr_array_index(array, middle);
        value = compare_func(&elem, target, extra_data);

        if (value > 0)
            right = middle - 1;
        else if (value < 0)
            left = middle + 1;
        else
        {
            result = middle;
            left = middle + 1;
        }
    }

    if (search_smaller_nearest == 1 && result == -1)
        result = left - 1;

    return result;
}
