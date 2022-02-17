#ifndef AUTOCOMPLETE_TREE_H
#define AUTOCOMPLETE_TREE_H

#include "node.h"

#if defined(_WIN32) || defined(__CYGWIN__) || defined(_WIN64)
#if defined(BUILD_SHARED)
#else
#define LIB
#endif
#else
#endif
struct tree {
    Node* head;
};
typedef struct tree Tree;
LIB Tree* tree_create(const char* filepath);
LIB void tree_free(Tree* tree);

#endif
