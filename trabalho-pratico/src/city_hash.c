#include <stdint.h>
#include <string.h>
#include "../includes/city_hash.h"

typedef enum city_index {
    BRAGA = 0,
    COIMBRA = 1,
    FARO = 2,
    LISBOA = 3,
    PORTO = 4,
    SETUBAL = 5,
    VILA_REAL = 6
} City_Index;

int city_hash(char *city) { // variation of djb2 string hash function to hash the cities of Portugal
    uint64_t result = 5381;
    unsigned char *p = (unsigned char *)city;

    while (*p)
        result = ((result << 5) ^ result) ^ (*p++);

    return result % 97;
}

/*
City values:
Aveiro => 33
Beja => 49
Braga => 96
Bragança => 8
Castelo branco => 11
Coimbra => 65
Évora => 38
Faro => 22
Guarda => 80
Leiria => 82
Lisboa => 3
Portalegre => 61
Porto => 73
Santarém => 31
Setúbal => 41
Viana do castelo => 7
Vila real => 2
Viseu => 35
*/

int get_city_index(char *city) {
    int hash = city_hash(city);

    int result = -1;

    switch (hash) {
    case 96:
        result = BRAGA;
        break;

    case 65:
        result = COIMBRA;
        break;

    case 22:
        result = FARO;
        break;

    case 3:
        result = LISBOA;
        break;

    case 73:
        result = PORTO;
        break;

    case 41:
        result = SETUBAL;
        break;

    case 2:
        result = VILA_REAL;
        break;
    }

    return result;
}

char *get_city_str(int index) {
    char *result = NULL;

    switch (index) {
    case BRAGA:
        result = "Braga";
        break;

    case COIMBRA:
        result = "Coimbra";
        break;

    case FARO:
        result = "Faro";
        break;

    case LISBOA:
        result = "Lisboa";
        break;

    case PORTO:
        result = "Porto";
        break;

    case SETUBAL:
        result = "Setúbal";
        break;

    case VILA_REAL:
        result = "Vila real";
        break;
    }

    return result;
}
