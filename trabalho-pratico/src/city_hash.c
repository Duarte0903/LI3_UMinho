#include <stdint.h>
#include <string.h>
#include "../includes/city_hash.h"

typedef enum city_index
{
    BRAGA = 0,
    COIMBRA = 1,
    FARO = 2,
    LISBOA = 3,
    PORTO = 4,
    SETUBAL = 5,
    VILA_REAL = 6
} City_Index;

int city_hash(char *city)
{ // variation of djb2 string hash function to hash the cities of Portugal
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

int get_city_index(char *city)
{
    int hash = city_hash(city);

    int result = -1;

    switch (hash)
    {
    case 74:
        if (!strcmp("Braga", city))
        {
            result = BRAGA;
        }
        break;

    case 46:
        if (!strcmp("Coimbra", city))
        {
            result = COIMBRA;
        }
        break;

    case 82:
        if (!strcmp("Faro", city))
        {
            result = FARO;
        }
        break;

    case 6:
        if (!strcmp("Lisboa", city))
        {
            result = LISBOA;
        }
        break;

    case 73:
        if (!strcmp("Porto", city))
        {
            result = PORTO;
        }
        break;

    case 25:
        if (!strcmp("Setúbal", city))
        {
            result = SETUBAL;
        }
        break;

    case 51:
        if (!strcmp("Vila real", city))
        {
            result = VILA_REAL;
        }
        break;
    }

    return result;
}
