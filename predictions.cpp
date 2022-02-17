#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "predictions.h"

char* token_create(char* str, unsigned str_len) {
    char* token = (char*)malloc(sizeof(char) * str_len + 1);
    if (token == NULL) {
        fprintf(stderr, "[ERROR] Bad token memory allocation\n");
        exit(0);
    }

    memcpy(token, str, str_len);
    token[str_len] = '\0';

    return token;
}

Tokens* split(char* str, char delimiter) {
    Tokens* vec = vector_create(1);

    char prev_char = delimiter;

    for (unsigned i = 0; str[i] != '\0'; i++) {

        if (prev_char == delimiter && str[i] != delimiter) {

            unsigned token_length = 0;

            while (str[i + token_length] != delimiter && str[i + token_length] != '\0') {
                token_length += 1;
            }

            vector_push(vec, token_create(str + i, token_length));

            i += token_length - 1;
        }

        prev_char = str[i];
    }

    if (prev_char == delimiter) {
        char* token = token_create((""), 1);
        vector_push(vec, token);
    }

    return vec;
}

Predictions* predictions_create(Tree* rules, char* input, char* optional_brackets) {
    Predictions* pred = (Predictions*)malloc(sizeof(Predictions));
    if (pred == NULL) {
        fprintf(stderr, "[ERROR] Bad prediction memory allocation\n");
        exit(0);
    }
    pred->type = EXACTLY;
    pred->tokens = vector_create(1);
    Tokens* tokens = split(input, ' ');

    Node* curr_node = rules->head;
    Node* prev_node = rules->head;
    Vector* curr_children = curr_node->children;
    if (contain_chars((char*)vector_get(tokens, tokens->length - 1), optional_brackets)) {
        pred->type = FAILURE;
        return pred;
    }

    for (unsigned i = 0; i < tokens->length - 1; i++) {
        for (unsigned j = 0; j < curr_children->length; j++) {
            Node* temp_node = (Node*)vector_get(curr_children, j);
            char* token = (char*)vector_get(tokens, i);

            if (strcmp(temp_node->token, token) == 0) {
                curr_node = temp_node;
                curr_children = curr_node->children;
                break;
            }
        }

        if (prev_node == curr_node) {
            pred->type = FAILURE;
            break;
        }

        prev_node = curr_node;
    }

    if (pred->type != FAILURE) {
        for (unsigned i = 0; i < curr_children->length; i++) {
            char* probably_token = ((Node*)vector_get(curr_children, i))->token;
            char* last_token = (char*)vector_get(tokens, tokens->length - 1);

            if (strncmp(last_token, probably_token, strlen(last_token)) == 0) {
                vector_push(pred->tokens, token_create(probably_token, (unsigned int)strlen(probably_token)));
            }
        }
    }

    if (pred->tokens->length > 0) {
        pred->type = EXACTLY;
    }
    else if (pred->type != FAILURE) {
        for (unsigned i = 0; i < curr_children->length; i++) {
            char* probably_token = ((Node*)vector_get(curr_children, i))->token;
            char* last_token = (char*)vector_get(tokens, tokens->length - 1);
            if (contain_chars(probably_token, optional_brackets)) {
                continue;
            }

            unsigned miss = 0;

            for (unsigned j = 0; j < strlen(last_token); j++) {
                if (last_token[j] != probably_token[j]) {
                    if (++miss == 2) {
                        break;
                    }
                }
            }

            if (miss < 2) {
                vector_push(pred->tokens, token_create(probably_token, (unsigned int)strlen(probably_token)));
            }
        }

        pred->type = pred->tokens->length > 0 ? PROBABLY : FAILURE;
    }

    tokens_free(tokens);

    return pred;
}

void predictions_free(Predictions* predict) {

    tokens_free(predict->tokens);

    free(predict);
}

void tokens_free(Tokens* tokens) {

    for (unsigned i = 0; i < tokens->length; i++) {
        free(vector_get(tokens, i));
    }

    vector_free(tokens);
}

int contain_chars(const char* str, const char* chars) {

    for (unsigned i = 0; str[i] != '\0'; i++) {
        for (unsigned j = 0; chars[j] != '\0'; j++) {
            if (str[i] == chars[j]) {
                return 1;
            }
        }
    }

    return 0;
}