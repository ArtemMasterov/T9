#ifndef AUTOCOMPLETE_NODE_H
#define AUTOCOMPLETE_NODE_H

#include "vector.h"

#if defined(_WIN32) || defined(__CYGWIN__) || defined(_WIN64)
#if defined(BUILD_SHARED)
#else
#define LIB
#endif
#else
#endif

struct node {
    char* token;
    struct vector* children;
};
typedef struct node Node;

LIB Node* node_create(char* token, unsigned token_length);

LIB void node_free(Node* node);

#endif
