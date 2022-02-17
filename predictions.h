#ifndef AUTOCOMPLETE_PREDICTIONS_H
#define AUTOCOMPLETE_PREDICTIONS_H

#if defined(_WIN32) || defined(__CYGWIN__) || defined(_WIN64)
#if defined(BUILD_SHARED)
#else
#define LIB
#endif
#else
#endif

#include "tree.h"
#include "vector.h"

typedef Vector Tokens;

enum predict_type {
    FAILURE = 0,
    EXACTLY = 1,
    PROBABLY = 2,
};
typedef enum predict_type PredictType;
struct predictions {
    PredictType type;
    Tokens* tokens;
};
typedef struct predictions Predictions;
LIB Predictions* predictions_create(Tree* rules, char* input, char* optional_brackets);
LIB void predictions_free(Predictions* predict);
LIB Tokens* split(char* str, char delimiter);
LIB char* token_create(char* str, unsigned str_len);
LIB void tokens_free(Tokens* tokens);
LIB int contain_chars(const char* str, const char* chars);

#endif