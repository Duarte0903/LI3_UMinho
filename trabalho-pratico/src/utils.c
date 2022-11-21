#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include "../includes/date.h"
#include "../includes/utils.h"

#define REF_DAY "9/10/2022"

// char **split_string(char *str, int n_elems)
// {
//     char **result = malloc(sizeof(char *) * n_elems);
//     char *token = NULL;
//     int index = 0;

//     while ((token = strsep(&str, ";")) != NULL)
//         result[index++] = token;

//     return result;
// }

// char **parse_line(char *line, int n_elems)
// {
//     char **result;
//     char *line_copy;
//     line_copy = strdup(line);
//     assert(line_copy != NULL);

//     result = split_string(line_copy, n_elems);

//     // printf("--> ");
//     // for(int i = 0; i < 7; i++)
//     //     printf("%s ", result[i]);

//     // free(save);
//     //  free(result);
//     return result;
// }

void free_str_array(char **arr, int n)
{
    int i = 0;
    for (i = 0; i < n; i++)
    {
        free(arr[i]);
    }
    free(arr);
}

// char *get_age(char *birth_date)
// {
//     char ref_age[] = "9/10/2022";
//     char *age_str = malloc(5 * sizeof(char));
//     unsigned short birth_days = date_to_int(birth_date);
//     unsigned short ref_days = date_to_int(ref_age);
//     unsigned short age_days = ref_days - birth_days;
//     unsigned short age = age_days / 365;
//     sprintf(age_str, "%hu", age);
//     return age_str;
// }

// char *strcat_driver_data(char **strArr)
// {
//     char *result = malloc(10000 * sizeof(char));
//     result[0] = '\0';
//     strcat(result, strArr[1]);
//     strcat(result, ";");
//     strcat(result, strArr[3]);
//     strcat(result, ";");
//     strcat(result, get_age(strArr[2]));
//     strcat(result, ";");
//     return result;
// }

// char *strcat_user_data(char **strArr)
// {
//     char *result = malloc(100 * sizeof(char));
//     result[0] = '\0';
//     strcat(result, strArr[1]);
//     strcat(result, ";");
//     strcat(result, strArr[2]);
//     strcat(result, ";");
//     strcat(result, get_age(strArr[3]));
//     strcat(result, ";");
//     return result;
// }

// char *strcat_user_data_q3(char **strArr)
// {
//     char *result = malloc(100 * sizeof(char));
//     result[0] = '\0';
//     strcat(result, strArr[0]);
//     strcat(result, ";");
//     strcat(result, strArr[1]);
//     strcat(result, ";");
//     return result;
// }

char *get_age(unsigned short birth_date)
{
    char *age_str = malloc(4 * sizeof(char));
    unsigned short ref_day = date_to_int(REF_DAY);
    unsigned short age = (ref_day - birth_date) / 365.25;
    sprintf(age_str, "%hu", age);
    return age_str;
}

// void init_str_to_0(char *str)
// {
//     str[0] = '\0';
// }

char *get_file(char *path, const char *file)
{
    char *result = malloc(strlen(path) + strlen(file) + 1);
    strcpy(result, path);
    strcat(result, file);
    return result;
}

void debug_parser(char **arr, int n)
{
    int i;
    printf("---> ");

    for (i = 0; i < n - 1; i++)
        printf("%s, ", arr[i]);

    printf("%s", arr[i]);
}

int str_to_int(char *str)
{
    char *end = NULL;
    errno = 0;
    int result = -1; // fix this
    long value = strtol(str, &end, 10);

    if (errno == 0 && *end == '\0') // Boundary check not needed since we are going to do an input validation
        result = (int)value;

    return result;
}

float str_to_float(char *str)
{
    char *end = NULL;
    errno = 0;
    float result = -1.0; //fix this
    float value = strtof(str, &end);

    if (errno == 0 && *end == '\0')
    {
        result = value;
    }

    return result;
}
