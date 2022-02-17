#ifndef AUTOCOMPLETE_VECTOR_H
#define AUTOCOMPLETE_VECTOR_H

#define MAX_OF(x, y) (((x) > (y)) ? (x) : (y))

#if defined(_WIN32) || defined(__CYGWIN__) || defined(_WIN64)
#if defined(BUILD_SHARED)
#else
#define LIB
#endif
#else
#endif
struct vector {
    void** data;
    unsigned capacity;
    unsigned length;
};
typedef struct vector Vector;

LIB Vector* vector_create(unsigned length);
LIB void* vector_get(Vector* vec, unsigned index);
LIB void vector_set(Vector* vec, unsigned index, void* item);
LIB void vector_push(Vector* vec, void* item);
LIB void vector_free(Vector* vec);

#endif
