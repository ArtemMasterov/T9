#include "terminal.h"

void color_print(char* text, COLOR_TYPE color) {
#if defined(OS_WINDOWS)
    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_console == NULL) {
        fprintf(stderr, "[ERROR] Couldn't handle terminal\n");
        exit(1);
    }

    CONSOLE_SCREEN_BUFFER_INFO console_info;
    COLOR_TYPE backup;

    if (GetConsoleScreenBufferInfo(h_console, &console_info) == 0) {
        fprintf(stderr, "[ERROR] Couldn't get terminal info\n");
        exit(1);
    }
    backup = console_info.wAttributes;


    if (SetConsoleTextAttribute(h_console, color) == 0) {
        fprintf(stderr, "[ERROR] Couldn't set terminal color\n");
        exit(1);
    }

    printf("%s", text);


    if (SetConsoleTextAttribute(h_console, backup) == 0) {
        fprintf(stderr, "[ERROR] Couldn't reset terminal color\n");
        exit(1);
    }
}
#endif

short terminal_width() {
#if defined(OS_WINDOWS)

    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_console == NULL) {
        fprintf(stderr, "[ERROR] Couldn't handle terminal\n");
        exit(1);
    }

    CONSOLE_SCREEN_BUFFER_INFO console_info;
    if (GetConsoleScreenBufferInfo(h_console, &console_info) == 0) {
        fprintf(stderr, "[ERROR] Couldn't get terminal info\n");
        exit(1);
    }

    return console_info.dwSize.X;
#endif
}

void clear_line() {
#if defined(OS_WINDOWS)

    short width = terminal_width();
    if (width < 1) {
        fprintf(stderr, "[ERROR] Size of terminal is too small\n");
        exit(1);
    }

    char* empty = (char*)malloc(sizeof(char) * width);
    memset(empty, ' ', width);
    empty[width - 1] = '\0';

    printf("\r%s\r", empty);

    free(empty);
#elif defined(OS_UNIX)
    printf("\033[2K\r");
#endif
}

void set_cursor_x(short x) {
#if defined(OS_WINDOWS)
    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_console == NULL) {
        fprintf(stderr, "[ERROR] Couldn't handle terminal\n");
        exit(1);
    }

    COORD xy;
    xy.X = x - 1;
    xy.Y = get_cursor_y();

    if (SetConsoleCursorPosition(h_console, xy) == 0) {
        fprintf(stderr, "[ERROR] Couldn't set terminal cursor position\n");
        exit(1);
    }
#elif defined(OS_UNIX)
    printf("\033[%d;%dH", get_cursor_y(), x);
#endif
}

short get_cursor_y() {
#if defined(OS_WINDOWS)
    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_console == NULL) {
        fprintf(stderr, "[ERROR] Couldn't handle terminal\n");
        exit(1);
    }

    CONSOLE_SCREEN_BUFFER_INFO console_info;
    if (GetConsoleScreenBufferInfo(h_console, &console_info) == 0) {
        fprintf(stderr, "[ERROR] Couldn't get terminal Y position\n");
        exit(1);
    }

    return console_info.dwCursorPosition.Y;

#endif
}