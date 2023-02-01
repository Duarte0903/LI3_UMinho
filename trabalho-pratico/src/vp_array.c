#include <stdlib.h>
#include <string.h>
#include "../includes/vp_array.h"

typedef struct vp_array {
    void **elements;
    int capacity;
    int size;
} VPA;

VPA *vpa_create(int capacity) {
    VPA *arr = malloc(sizeof(struct vp_array));
    arr->elements = malloc(capacity * sizeof(void *));
    arr->capacity = capacity;
    arr->size = 0;
    return arr;
}

void vpa_add(VPA *arr, void *value, size_t type_size) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->elements = realloc(arr->elements, arr->capacity * sizeof(void *));
    }
    /*
    arr->elements[arr->size] = malloc(type_size);
    memcpy(arr->elements[arr->size], value, type_size);
    arr->size++;
    */
    void *value_ptr = malloc(type_size);
    memcpy(value_ptr, value, type_size);

    arr->elements[arr->size] = value_ptr;
    arr->size++;
}

void *vpa_get(VPA *arr, int index) {
    if (index >= arr->size)
        return NULL;
    return arr->elements[index];
}

void vpa_free(VPA *arr) {
    for (int i = 0; i < arr->size; i++)
        free(arr->elements[i]);
    free(arr->elements);
    free(arr);
}
