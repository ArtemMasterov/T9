#ifndef AUTOCOMPLETE_TERMINAL_H
#define AUTOCOMPLETE_TERMINAL_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(__CYGWIN__) || defined(_WIN64)
#ifndef OS_WINDOWS
#define OS_WINDOWS
#define COLOR_TYPE uint16_t
#if defined(BUILD_SHARED)
#define LIB extern __declspec(dllexport)
#else
#define LIB
#endif

#include <windows.h>
#include <conio.h>
#endif
#elif defined(__APPLE__) || defined(__unix__) || defined(__unix) || defined(unix) || defined(__linux__)
#ifndef OS_UNIX
#define OS_UNIX
#define COLOR_TYPE char*
#define LIB extern __attribute__((visibility("default")))

#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#endif
#else
#error Unknown environment!
#endif

#if defined(OS_WINDOWS)
#define DEFAULT_TITLE_COLOR 160
#define DEFAULT_PREDICT_COLOR 8
#define DEFAULT_MAIN_COLOR 7
#endif
LIB short terminal_width();
LIB void color_print(char* text, COLOR_TYPE color);
LIB void clear_line();
LIB void set_cursor_x(short x);
LIB short get_cursor_y();
#endif