#ifdef _MSC_VER
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

#include <stdio.h>
#include "autocomplete1.h"

int main() {
    Tree* rules = tree_create("../../../../example.config");

#if defined(OS_WINDOWS)
    COLOR_TYPE title_color = 160;
    COLOR_TYPE predict_color = 8;
    COLOR_TYPE main_color = 7;
#endif

    char* optional_brackets = "[{<";

    fprintf(
        stderr,
        "Attention! Please run the executable file only\n"
        "           through the command line!\n\n"

        "- To switch the prompts press UP or DOWN arrow.\n"
        "- To move cursor press LEFT or RIGHT arrow.\n"
        "- To edit input press DELETE or BACKSPACE key.\n"
        "- To apply current prompt press TAB key.\n\n"
    );

    unsigned command_counter = 0;
    char title[64] = { 0 };

    while (1) {
        sprintf(title, "%s [%u]", "git", command_counter);
        char* str = custom_input(rules, title, title_color, predict_color, main_color, optional_brackets);
        printf("\n%s\n", str);
        if (strcmp(str, "") == 0) {
            free(str);
            break;
        }

        free(str);
        command_counter += 1;
    }
    tree_free(rules);

    return 0;
}