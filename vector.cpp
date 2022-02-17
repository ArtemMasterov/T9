#include <stdlib.h>
#include <stdio.h>

#include "vector.h"

Vector* vector_create(unsigned length) {
    Vector* vec = (Vector*)malloc(sizeof(Vector));
    if (vec == NULL) {
        fprintf(stderr, "[ERROR] Bad vector memory allocation\n");
        exit(1);
    }
    vec->length = 0;
    vec->capacity = MAX_OF(length, 1);
    vec->data = (void**)malloc(sizeof(void*) * length);
    if (vec->data == NULL) {
        fprintf(stderr, "[ERROR] Bad vector memory allocation\n");
        exit(1);
    }

    return vec;
}

void* vector_get(Vector* vec, unsigned index) {
    if (index >= vec->length) {
        fprintf(stderr, "[ERROR] Out of bounds an vector\n");
        exit(1);
    }
    return vec->data[index];
}

void vector_set(Vector* vec, unsigned index, void* item) {
    if (index >= vec->length) {
        fprintf(stderr, "[ERROR] Out of bounds an vector\n");
        exit(1);
    }

    vec->data[index] = item;
}

void vector_push(Vector* vec, void* item) {
    if (vec->length >= vec->capacity) {
        vec->capacity = (vec->capacity + 1) * 2;
        vec->data = (void**)realloc(vec->data, vec->capacity * sizeof(void*));
        if (vec->data == NULL) {
            fprintf(stderr, "[ERROR] Bad vector memory reallocation\n");
            exit(1);
        }
    }

    vec->data[vec->length++] = item;
}

void vector_free(Vector* vec) {
    free(vec->data);
    free(vec);
}