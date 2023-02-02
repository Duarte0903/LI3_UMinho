#include <glib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include "../includes/rides-catalog.h"
#include "../includes/drivers-catalog.h"
#include "../includes/users-catalog.h"
#include "../includes/ride.h"
#include "../includes/utils.h"
#include "../includes/date.h"

#define N_USER_STATS 4
#define N_DRIVER_STATS 3

typedef struct rides_catalog {
    GPtrArray *rides_array;
    enum sort_mode {
        UNSORTED,
        DATE,
        CITY,
        CITY_DRIVER_ID,
        ACCOUNT_AGE
    } sort_mode;
} *Rides_Catalog;

typedef struct driver_average_by_city {
    char *driver_id;
    double average_rating;
} *Driver_in_City;

Driver_in_City new_driver_in_city() {
    Driver_in_City driver = malloc(sizeof(struct driver_average_by_city));
    driver->driver_id = NULL;
    return driver;
}

void glib_wrapper_free_ride(gpointer ride) {
    free_ride(ride);
}

void free_driver_by_city(Driver_in_City driver) {
    free(driver->driver_id);
    free(driver);
}

void glib_wrapper_free_driver_in_city(gpointer driver) {
    free_driver_by_city(driver);
}

Rides_Catalog create_rides_catalog() {
    Rides_Catalog catalog = malloc(sizeof(struct rides_catalog));

    catalog->rides_array = g_ptr_array_new_with_free_func(glib_wrapper_free_ride);
    catalog->sort_mode = UNSORTED;

    return catalog;
}

int is_valid_ride(char **fields) {
    if (IS_EMPTY(fields[0]) || IS_EMPTY(fields[2]) || IS_EMPTY(fields[3]) || IS_EMPTY(fields[4]))
        return 0;

    int day, month, year;

    if (!(sscanf(fields[1], "%2d/%2d/%4d", &day, &month, &year) == 3 && is_valid_date(day, month, year)))
        return 0;

    if (IS_EMPTY(fields[5]) || !is_positive_integer(fields[5]))
        return 0;

    if (IS_EMPTY(fields[6]) || !is_positive_integer(fields[6]))
        return 0;

    if (IS_EMPTY(fields[7]) || !is_positive_integer(fields[7]))
        return 0;

    if (IS_EMPTY(fields[8]) || !is_non_negative_double(fields[8]))
        return 0;

    return 1;
}

double calculate_ride_cost(char *car_class, unsigned short distance) {
    double result = 0.0;

    if (!strcasecmp(car_class, "basic"))
        result += 3.25 + 0.62 * distance;
    else if (!strcasecmp(car_class, "green"))
        result += 4 + 0.79 * distance;
    else
        result += 5.20 + 0.94 * distance;

    return result;
}

void **get_user_stats_from_ride(unsigned short ride_user_score, double ride_cost_w_tip, unsigned short ride_distance, unsigned short ride_date) {
    void **stats = malloc(N_USER_STATS * sizeof(void *));

    if (!stats) {
        perror("Error: Failed to allocate memory!\n");
        exit(EXIT_FAILURE);
    }

    unsigned short *score = malloc(sizeof(unsigned short)); // otimizar ? (não sei se dá)
    double *cost = malloc(sizeof(double));
    unsigned short *distance = malloc(sizeof(unsigned short));
    unsigned short *date = malloc(sizeof(unsigned short));

    *score = ride_user_score;
    *cost = ride_cost_w_tip;
    *distance = ride_distance;
    *date = ride_date;

    stats[0] = score;
    stats[1] = cost;
    stats[2] = distance;
    stats[3] = date;

    return stats;
}

void **get_driver_stats_from_ride(unsigned short ride_driver_score, double ride_cost_w_tip, unsigned short ride_date) {
    void **stats = malloc(N_DRIVER_STATS * sizeof(void *));

    if (!stats) {
        perror("Error: Failed to allocate memory!\n");
        exit(EXIT_FAILURE);
    }

    unsigned short *score = malloc(sizeof(unsigned short)); // otimizar ? (não sei se dá)
    double *cost = malloc(sizeof(double));
    unsigned short *date = malloc(sizeof(unsigned short));

    *score = ride_driver_score;
    *cost = ride_cost_w_tip;
    *date = ride_date;

    stats[0] = score;
    stats[1] = cost;
    stats[2] = date;

    return stats;
}

void insert_ride_in_catalog(char **fields, va_list args) {
    Rides_Catalog rides_catalog = va_arg(args, Rides_Catalog);
    Users_Catalog users_catalog = va_arg(args, Users_Catalog);
    Drivers_Catalog drivers_catalog = va_arg(args, Drivers_Catalog);

    Ride ride = create_ride(fields);
    g_ptr_array_add(rides_catalog->rides_array, ride);

    char *username = get_ride_user(ride);
    char *driver_id = get_ride_driver_id(ride);
    unsigned short ride_distance = get_ride_distance(ride);
    char *ride_car_class = get_ride_car_class(driver_id, drivers_catalog);
    double ride_cost = calculate_ride_cost(ride_car_class, ride_distance);
    set_ride_cost(ride, ride_cost);
    free(ride_car_class);

    double ride_tip = get_ride_tip(ride);
    double ride_cost_w_tip = ride_cost + ride_tip;
    unsigned short ride_user_score = get_ride_user_score(ride);
    unsigned short ride_driver_score = get_ride_driver_score(ride);
    unsigned short ride_date = get_ride_date(ride);

    void **user_stats_fields = get_user_stats_from_ride(ride_user_score, ride_cost_w_tip, ride_distance, ride_date);
    update_user_stats(username, user_stats_fields, users_catalog);
    free(username);
    for (int i = 0; i < N_USER_STATS; i++)
        free(user_stats_fields[i]);
    free(user_stats_fields);

    void **driver_stats_fields = get_driver_stats_from_ride(ride_driver_score, ride_cost_w_tip, ride_date);
    update_driver_stats(driver_id, driver_stats_fields, drivers_catalog);
    free(driver_id);
    for (int i = 0; i < N_DRIVER_STATS; i++)
        free(driver_stats_fields[i]);
    free(driver_stats_fields);
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> COMPARE FUNCTIONS TO SORT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< //

static gint compare_rides_by_date(gconstpointer r1, gconstpointer r2) {
    Ride ride1 = *(Ride *)r1;
    Ride ride2 = *(Ride *)r2;

    unsigned short date1 = get_ride_date(ride1);
    unsigned short date2 = get_ride_date(ride2);

    return (date1 < date2) ? -1 : (date1 > date2);
}

static gint compare_ride_by_city(gconstpointer r1, gconstpointer r2) {
    Ride ride1 = *(Ride *)r1;
    Ride ride2 = *(Ride *)r2;

    char *city1 = get_ride_city(ride1);
    char *city2 = get_ride_city(ride2);

    int result = strcmp(city1, city2);

    free(city1);
    free(city2);

    return result;
}

static gint compare_ride_by_city_and_driver_id(gconstpointer r1, gconstpointer r2) {
    Ride ride1 = *(Ride *)r1;
    Ride ride2 = *(Ride *)r2;

    char *city1 = get_ride_city(ride1);
    char *city2 = get_ride_city(ride2);

    int result = strcmp(city1, city2);

    if (result == 0) {
        char *id_1 = get_ride_driver_id(ride1);
        char *id_2 = get_ride_driver_id(ride2);
        result = strcmp(id_1, id_2);
        free(id_1);
        free(id_2);
    }

    free(city1);
    free(city2);

    return result;
}

static gint compare_driver_average_in_city(gconstpointer d1, gconstpointer d2) {
    Driver_in_City driver_1 = *(Driver_in_City *)d1;
    Driver_in_City driver_2 = *(Driver_in_City *)d2;

    double diff = driver_1->average_rating - driver_2->average_rating;
    int result;

    // Descending order
    if (diff < 0)
        result = 1;
    else if (diff > 0)
        result = -1;
    else
        result = -strcmp(driver_1->driver_id, driver_2->driver_id);

    return result;
}

static gint compare_rides_by_account_age(gconstpointer r1, gconstpointer r2, gpointer array_with_extra_data) {
    int result = 0;
    Ride ride1 = *(Ride *)r1;
    Ride ride2 = *(Ride *)r2;
    GPtrArray *array = *(GPtrArray **)array_with_extra_data;
    Users_Catalog users_catalog = g_ptr_array_index(array, 0);
    Drivers_Catalog drivers_catalog = g_ptr_array_index(array, 1);

    char *r1_driver_id = get_ride_driver_id(ride1);
    char *r1_user_username = get_ride_user(ride1);
    unsigned short r1_driver_acc_age = get_driver_account_age_w_id(r1_driver_id, drivers_catalog);
    unsigned short r1_user_acc_age = get_user_account_age_w_username(r1_user_username, users_catalog);

    char *r2_driver_id = get_ride_driver_id(ride2);
    char *r2_user_username = get_ride_user(ride2);
    unsigned short r2_driver_acc_age = get_driver_account_age_w_id(r2_driver_id, drivers_catalog);
    unsigned short r2_user_acc_age = get_user_account_age_w_username(r2_user_username, users_catalog);

    // sort by driver, user only if drivers have the same account age, ride id only if both ages are the same
    // Descending order: driver account age, user account age
    // Ascending order: ride_id
    if (r1_driver_acc_age > r2_driver_acc_age || (r1_driver_acc_age == r2_driver_acc_age && r1_user_acc_age > r2_user_acc_age))
        result = -1;
    else if (r1_driver_acc_age < r2_driver_acc_age || (r1_driver_acc_age == r2_driver_acc_age && r1_user_acc_age < r2_user_acc_age))
        result = 1;
    else {
        char *r1_id = get_ride_id(ride1);
        char *r2_id = get_ride_id(ride2);
        result = strcmp(r1_id, r2_id);
        free(r1_id);
        free(r2_id);
    }

    free(r1_driver_id);
    free(r1_user_username);
    free(r2_driver_id);
    free(r2_user_username);

    return result;
}

static gint compare_rides_by_distance_w_tip(gconstpointer r1, gconstpointer r2) {
    Ride ride1 = *(Ride *)r1;
    Ride ride2 = *(Ride *)r2;

    double tip1 = get_ride_tip(ride1);
    double tip2 = get_ride_tip(ride2);

    if (tip1 > 0.0 && nearly_equal_fp_numbers(tip2, 0.0, 0.00001))
        return -1;

    if (nearly_equal_fp_numbers(tip1, 0.0, 0.00001) && tip2 > 0.0)
        return 1;

    unsigned short distance1 = get_ride_distance(ride1);
    unsigned short date1 = get_ride_date(ride1);

    unsigned short distance2 = get_ride_distance(ride2);
    unsigned short date2 = get_ride_date(ride2);

    int result;

    if (distance1 < distance2 || (distance1 == distance2 && date1 < date2))
        result = 1;
    else if (distance1 > distance2 || (distance1 == distance2 && date1 > date2))
        result = -1;
    else {
        char *ride_id1 = get_ride_id(ride1);
        char *ride_id2 = get_ride_id(ride2);
        result = -strcmp(ride_id1, ride_id2);
        free(ride_id1);
        free(ride_id2);
    }

    return result;
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> COMPARE FUNCTIONS TO BS <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< //

static gint compare_ride_date_bsearch(gconstpointer r, gconstpointer d) {
    Ride ride = *(Ride *)r;
    unsigned short date = *(unsigned short *)d;

    unsigned short ride_date = get_ride_date(ride);

    return (ride_date < date) ? -1 : (ride_date > date);
}

static gint compare_ride_city_bsearch(gconstpointer r1, gconstpointer c) {
    Ride ride = *(Ride *)r1;
    char *city = *(char **)c;

    char *ride_city = get_ride_city(ride);

    int result = strcmp(ride_city, city);

    free(ride_city);

    return result;
}

static gint compare_ride_account_age_bsearch(gconstpointer r1, gconstpointer minimum_age, gpointer catalog_pointers) {
    Ride ride1 = *(Ride *)r1;
    unsigned short *min_age_ptr = (unsigned short *)minimum_age;
    GPtrArray *array = *(GPtrArray **)catalog_pointers;
    Drivers_Catalog drivers_catalog = g_ptr_array_index(array, 1);

    char *ride_driver_id = get_ride_driver_id(ride1);
    int driver_acc_age_days = get_driver_account_age_w_id(ride_driver_id, drivers_catalog);
    int driver_acc_age_years = get_age_no_ref(driver_acc_age_days);

    free(ride_driver_id);

    return (driver_acc_age_years > *min_age_ptr) ? -1 : (driver_acc_age_years < *min_age_ptr);
}

static gint compare_ride_tip_w_zero_bsearch(gconstpointer r1, gconstpointer z) {
    Ride ride = *(Ride *)r1;
    double zero = *(double *)z;

    double tip = get_ride_tip(ride);

    return (tip > zero) ? 0 : 1;
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> SORT FUNCTIONS <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< //

void sort_rides_by_date(Rides_Catalog catalog) {
    if (catalog->sort_mode != DATE) {
        g_ptr_array_sort(catalog->rides_array, compare_rides_by_date);
        catalog->sort_mode = DATE;
    }
}

void sort_rides_by_city(Rides_Catalog catalog) {
    if (catalog->sort_mode != CITY) {
        g_ptr_array_sort(catalog->rides_array, compare_ride_by_city);
        catalog->sort_mode = CITY;
    }
}

void sort_rides_by_city_and_driver_id(Rides_Catalog catalog) {
    if (catalog->sort_mode != CITY_DRIVER_ID) {
        g_ptr_array_sort(catalog->rides_array, compare_ride_by_city_and_driver_id);
        catalog->sort_mode = CITY_DRIVER_ID;
    }
}

void sort_rides_by_account_age(Rides_Catalog catalog, gpointer array_with_catalog_pointers) {
    if (catalog->sort_mode != ACCOUNT_AGE) {
        g_ptr_array_sort_with_data(catalog->rides_array, compare_rides_by_account_age, array_with_catalog_pointers);
        catalog->sort_mode = ACCOUNT_AGE;
    }
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> QUERY OUTPUT FUNCTIONS <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< //

char *get_q4(char *city, Rides_Catalog catalog) {
    int first_elem = first_occurrence_ptr_array_bsearch(catalog->rides_array, compare_ride_city_bsearch, &city, 0);

    if (first_elem == -1)
        return NULL;

    int last_elem = last_occurrence_ptr_array_bsearch(catalog->rides_array, compare_ride_city_bsearch, &city, 0);
    int n_rides_in_city = last_elem - first_elem + 1;
    double average_price_in_city = 0.0;

    for (int i = first_elem; i <= last_elem; i++) {
        average_price_in_city += get_ride_cost(g_ptr_array_index(catalog->rides_array, i));
    }
    average_price_in_city /= n_rides_in_city;

    char *result = malloc(12 * sizeof(char));
    sprintf(result, "%.3f\n", average_price_in_city);

    return result;
}

char *get_q5(unsigned short start_date, unsigned short end_date, Rides_Catalog catalog) {
    int first_elem = first_occurrence_ptr_array_bsearch(catalog->rides_array, compare_ride_date_bsearch, &start_date, 1);
    int last_elem = last_occurrence_ptr_array_bsearch(catalog->rides_array, compare_ride_date_bsearch, &end_date, 1);

    if (first_elem == -1 || last_elem == -1) // the dates do not exist in the array + out of bounds
        return NULL;

    int n_rides = last_elem - first_elem + 1;
    double average_price = 0.0;

    for (int i = first_elem; i <= last_elem; i++) {
        average_price += get_ride_cost(g_ptr_array_index(catalog->rides_array, i));
    }
    average_price /= n_rides;

    char *result = malloc(12 * sizeof(char)); // 10 average price, 1 \0
    sprintf(result, "%.3f\n", average_price);

    return result;
}

char *get_q6(char *city, unsigned short start_date, unsigned short end_date, Rides_Catalog catalog) {
    int first_elem = first_occurrence_ptr_array_bsearch(catalog->rides_array, compare_ride_date_bsearch, &start_date, 1);
    int last_elem = last_occurrence_ptr_array_bsearch(catalog->rides_array, compare_ride_date_bsearch, &end_date, 1);

    if (first_elem == -1 || last_elem == -1) // the dates do not exist in the array + out of bounds
        return NULL;

    int n_rides_in_city = 0;
    double average_distance_in_city = 0.0;

    for (int i = first_elem; i <= last_elem; i++) {
        Ride ride = g_ptr_array_index(catalog->rides_array, i);
        char *ride_city = get_ride_city(ride);

        if (strcmp(ride_city, city) == 0) {
            n_rides_in_city++;
            average_distance_in_city += get_ride_distance(ride);
        }

        free(ride_city);
    }

    if (n_rides_in_city == 0)
        return NULL;

    average_distance_in_city /= n_rides_in_city;

    char *result = malloc(12 * sizeof(char));
    sprintf(result, "%.3f\n", average_distance_in_city);

    return result;
}

char *get_q7(unsigned short n_drivers, char *city, va_list args) {
    (void)va_arg(args, Users_Catalog);
    Drivers_Catalog drivers_catalog = va_arg(args, Drivers_Catalog);
    Rides_Catalog rides_catalog = va_arg(args, Rides_Catalog);

    int first_elem = first_occurrence_ptr_array_bsearch(rides_catalog->rides_array, compare_ride_city_bsearch, &city, 0);
    int last_elem = last_occurrence_ptr_array_bsearch(rides_catalog->rides_array, compare_ride_city_bsearch, &city, 0);

    if (first_elem == -1 || last_elem == -1)
        return NULL;

    GPtrArray *results_array = g_ptr_array_new_with_free_func(glib_wrapper_free_driver_in_city);

    Ride ride = g_ptr_array_index(rides_catalog->rides_array, first_elem);
    char *driver_id = get_ride_driver_id(ride);
    unsigned short rating = get_ride_driver_score(ride);

    double average_rating = rating;
    int n_rides = 1;
    char *prev_id = strdup(driver_id);

    free(driver_id);

    for (int i = first_elem + 1; i <= last_elem; i++) {
        ride = g_ptr_array_index(rides_catalog->rides_array, i);
        driver_id = get_ride_driver_id(ride);
        rating = get_ride_driver_score(ride);

        if (strcmp(driver_id, prev_id) == 0) {
            average_rating += rating;
            n_rides++;
        } else {
            bool account_status = get_driver_account_status_id(prev_id, drivers_catalog);
            if (account_status) {
                Driver_in_City driver = new_driver_in_city();
                driver->driver_id = strdup(prev_id);
                driver->average_rating = average_rating / n_rides;
                g_ptr_array_add(results_array, driver);
            }

            free(prev_id);
            average_rating = rating;
            n_rides = 1;
            prev_id = strdup(driver_id);
        }

        free(driver_id);
    }

    free(prev_id);

    g_ptr_array_sort(results_array, compare_driver_average_in_city);

    if (n_drivers > (int)results_array->len)
        n_drivers = results_array->len;

    char *result = NULL;
    size_t result_size = 0;
    FILE *stream = open_memstream(&result, &result_size);

    for (int i = 0; i < n_drivers; i++) {
        Driver_in_City driver = g_ptr_array_index(results_array, i);
        char *name = get_driver_name_id(driver->driver_id, drivers_catalog);
        fprintf(stream, "%s;%s;%.3f\n", driver->driver_id, name, driver->average_rating);
        free(name);
    }

    fclose(stream);
    g_ptr_array_free(results_array, TRUE);

    return result;
}

char *get_q8(char *gender, int minimum_age, Rides_Catalog rides_catalog, gpointer catalog_pointer_extra_data) {
    GPtrArray *array_extra_data = *(GPtrArray **)catalog_pointer_extra_data;
    Users_Catalog users_catalog = g_ptr_array_index(array_extra_data, 0);
    Drivers_Catalog drivers_catalog = g_ptr_array_index(array_extra_data, 1);

    int last_elem_index = last_occurrence_ptr_array_bsearch_with_data(rides_catalog->rides_array, compare_ride_account_age_bsearch, &minimum_age, 0, catalog_pointer_extra_data);
    if (last_elem_index == -1)
        return NULL;

    char *result = NULL;
    size_t result_size = 0;
    FILE *stream = open_memstream(&result, &result_size);

    for (int i = 0; i <= last_elem_index; i++) {
        Ride ride = g_ptr_array_index(rides_catalog->rides_array, i);
        char *driver_id = get_ride_driver_id(ride);
        char *user_username = get_ride_user(ride);
        char *driver_gender = get_driver_gender_id(driver_id, drivers_catalog);
        char *user_gender = get_user_gender_username(user_username, users_catalog);

        if (!strcmp(driver_gender, user_gender) && !strcmp(user_gender, gender)) {
            bool driver_account_status = get_driver_account_status_id(driver_id, drivers_catalog);
            bool user_account_status = get_user_account_status_username(user_username, users_catalog);

            if (user_account_status && driver_account_status) {
                unsigned short user_account_age_years = get_age_no_ref(get_user_account_age_w_username(user_username, users_catalog));

                if (user_account_age_years >= minimum_age) {
                    char *driver_name = get_driver_name_id(driver_id, drivers_catalog);
                    char *user_name = get_user_name_username(user_username, users_catalog);
                    fprintf(stream, "%s;%s;%s;%s\n", driver_id, driver_name, user_username, user_name);
                    free(driver_name);
                    free(user_name);
                }
            }
        }

        free(driver_id);
        free(user_username);
        free(driver_gender);
        free(user_gender);
    }

    fclose(stream);

    return result;
}

char *get_q9(unsigned short start_date, unsigned short end_date, Rides_Catalog catalog) {
    int first_elem = first_occurrence_ptr_array_bsearch(catalog->rides_array, compare_ride_date_bsearch, &start_date, 1);
    int last_elem = last_occurrence_ptr_array_bsearch(catalog->rides_array, compare_ride_date_bsearch, &end_date, 1);

    if (first_elem == -1 || last_elem == -1) // the dates do not exist in the array + out of bounds
        return NULL;

    GPtrArray *results_array = g_ptr_array_new();

    for (int i = first_elem; i <= last_elem; i++) { // reverse?
        Ride ride = g_ptr_array_index(catalog->rides_array, i);
        g_ptr_array_add(results_array, ride);
    }

    g_ptr_array_sort(results_array, compare_rides_by_distance_w_tip);

    double zero = 0.0;
    int last_ride = last_occurrence_ptr_array_bsearch(results_array, compare_ride_tip_w_zero_bsearch, &zero, 0);

    char *result = NULL;
    size_t result_size = 0;
    FILE *stream = open_memstream(&result, &result_size);
    char *ride_id = NULL, *date = NULL, *city = NULL;
    unsigned short distance;
    double tip;

    for (int i = 0; i <= last_ride; i++) {
        Ride ride = g_ptr_array_index(results_array, i);
        ride_id = get_ride_id(ride);
        date = int_to_date(get_ride_date(ride));
        distance = get_ride_distance(ride);
        city = get_ride_city(ride);
        tip = get_ride_tip(ride);
        fprintf(stream, "%s;%s;%hu;%s;%.3f\n", ride_id, date, distance, city, tip);
        free(ride_id);
        free(date);
        free(city);
    }

    fclose(stream);

    g_ptr_array_free(results_array, TRUE);

    return result;
}

void free_rides_catalog(Rides_Catalog catalog) {
    g_ptr_array_free(catalog->rides_array, TRUE);
    free(catalog);
}
