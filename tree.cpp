#ifdef _MSC_VER
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

Tree* tree_create(const char* filepath) {
    int character;

    unsigned buff_length = 0;
    unsigned buff_counter = 0;


    FILE* config = fopen(filepath, "r");
    if (config == NULL) {
        fprintf(stderr, "[ERROR] Can't open file %s\n", filepath);
        exit(0);
    }


    character = fgetc(config);
    if (character == EOF) {
        fclose(config);
        fprintf(stderr, "[ERROR] %s file is empty\n", filepath);
        exit(0);
    }
    ungetc(character, config);


    while (character != EOF) {
        buff_counter = 0;

        while (character == ' ') {
            character = fgetc(config);
        }


        while (character != '\n' && character != '\r' && character != EOF) {
            buff_counter += 1;
            character = fgetc(config);
        }


        if (buff_counter > buff_length) {

            buff_length = buff_counter + 1;
        }

        while (character != ' ' && character != EOF) {
            character = fgetc(config);
        }
    }


    char* buff = (char*)malloc(sizeof(char) * buff_length);
    if (buff == NULL) {
        fprintf(stderr, "[ERROR] Bad buffer memory allocation\n");
        fclose(config);
        exit(1);
    }
    unsigned buff_i;

    unsigned space_counter;       
    unsigned tab_length = 0;      
    unsigned tab_count;           
    unsigned line_counter = 0;    

    Tree* tree = (Tree*)malloc(sizeof(Tree));
    if (tree == NULL) {
        fprintf(stderr, "[ERROR] Bad tree memory allocation\n");
        fclose(config);
        free(buff);
        exit(1);
    }
    tree->head = node_create(0, 1);
    Vector* root_nodes = vector_create(1);
    vector_push(root_nodes, (void*)tree->head);
    fseek(config, SEEK_SET, 0);

    int error = 0;


    character = fgetc(config);
    while (character != EOF) {
        line_counter += 1;
        space_counter = 0;
        buff_i = 0;


        if (character == '\t') {
            fprintf(stderr, "[ERROR] Tab character detected in line %d, use a sequence of spaces\n", line_counter);
            error = 1;
            break;
        }


        while (character == ' ') {
            space_counter += 1;
            character = fgetc(config);
        }


        if (space_counter > tab_length && tab_length == 0) {
            tab_length = space_counter;
        }

        if (space_counter % MAX_OF(tab_length, 1) != 0) {
            fprintf(stderr, "[ERROR] Incorrect tab length in line %d\n", line_counter);
            error = 1;
            break;
        }


        tab_count = space_counter / MAX_OF(tab_length, 1);


        while (character != '\n' && character != '\r' && character != EOF) {

            if (character == ' ' || character == '\t') {
                error = 1;
                break;
            }

            buff[buff_i++] = (char)character;
            character = fgetc(config);
        }


        if (error) {
            fprintf(stderr, "[ERROR] Token in config file must not have spaces, line %d\n", line_counter);
            break;
        }

        buff[buff_i] = '\0';


        Node* n = node_create(buff, buff_length);


        if (tab_count >= root_nodes->length) {
            fprintf(stderr, "[ERROR] The token on line %d does not belong to any token\n", line_counter);
            error = 1;
            break;
        }
        vector_push(((Node*)vector_get(root_nodes, tab_count))->children, (void*)n);


        if (tab_count + 1 < root_nodes->length) {
            vector_set(root_nodes, tab_count + 1, n);
        }
        else if (tab_count + 1 == root_nodes->length) {
            vector_push(root_nodes, (void*)n);
        }


        while (character != ' ' && character != EOF) {
            character = fgetc(config);
        }
    }


    fclose(config);
    vector_free(root_nodes);
    free(buff);


    if (error) {
        tree_free(tree);
        exit(1);
    }


    return tree;
}

void tree_free(Tree* t) {

    node_free(t->head);
    free(t);
}