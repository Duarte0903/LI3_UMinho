#ifndef VP_ARRAY_H
#define VP_ARRAY_H

typedef struct vp_array VPA;

VPA *vpa_create(int capacity);

void vpa_add(VPA *arr, void *value, size_t type_size);

void *vpa_get(VPA *arr, int index);

void vpa_free(VPA *arr);

#endif