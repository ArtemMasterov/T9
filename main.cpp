#include <stdio.h>

#include "autocomplete.h"

int main() {

    Tree* rules = tree_create("../example.config");

    fprintf(
        stderr,
        "Attention! Please run the executable file only\n"
        "           through the command line!\n\n"

        "- To switch the prompts press UP or DOWN arrow.\n"
        "- To move cursor press LEFT or RIGHT arrow.\n"
        "- To edit input press DELETE or BACKSPACE key.\n"
        "- To apply current prompt press TAB key.\n\n"
    );

    while (1) {
        char* str = input(rules);
        printf("\n%s\n", str);
        if (strcmp(str, "") == 0) {
            free(str);
            break;
        }

        free(str);
    }

    tree_free(rules);

    return 0;
}