#include <stdint.h>
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

    return result % 89;
}

/*
City values:
Aveiro => 60
Beja => 79
Braga => 74
Bragança => 30
Castelo branco => 11
Coimbra => 46
Évora => 17
Faro => 82
Guarda => 48
Leiria => 22
Lisboa => 6
Portalegre => 21
Porto => 73
Santarém => 56
Setúbal => 25
Viana do castelo => 15
Vila real => 51
Viseu => 14
*/

int get_city_index(char *city) {
    int hash = city_hash(city);

    int result = -1;

    switch (hash) {
    case 74:
        result = BRAGA;
        break;

    case 46:
        result = COIMBRA;
        break;

    case 82:
        result = FARO;
        break;

    case 6:
        result = LISBOA;
        break;

    case 73:
        result = PORTO;
        break;

    case 25:
        result = SETUBAL;
        break;

    case 51:
        result = VILA_REAL;
        break;
    }

    return result;
}
