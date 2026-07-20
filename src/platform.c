/**
 * @file platform.c
 * @brief Implementation of the platform abstraction layer.
 *
 * All OS-specific headers and calls are confined to this file, guarded by
 * the standard `_WIN32` predefined macro. Every function here has both a
 * Windows and a POSIX (Linux/macOS) implementation, selected at compile
 * time — no other file in the project needs to know which OS it is
 * running on.
 */

#include "platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

void platform_clear_screen(void) {
#ifdef _WIN32
    (void)system("cls");
#else
    (void)system("clear");
#endif
}

void platform_wait_for_keypress(void) {
#ifdef _WIN32
    (void)_getch();
#else
    struct termios old_settings;
    struct termios raw_settings;

    if (tcgetattr(STDIN_FILENO, &old_settings) != 0) {
        /* Fall back to a plain blocking read if raw mode is unavailable. */
        (void)getchar();
        return;
    }

    raw_settings = old_settings;
    raw_settings.c_lflag &= ~((tcflag_t)ICANON | (tcflag_t)ECHO);

    (void)tcsetattr(STDIN_FILENO, TCSANOW, &raw_settings);
    (void)getchar();
    (void)tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
#endif
}

void platform_get_current_time(SystemTime *out_time) {
    time_t raw_time;
    struct tm *local_time;

    if (out_time == NULL) {
        return;
    }

    raw_time = time(NULL);
    local_time = localtime(&raw_time);

    if (local_time == NULL) {
        out_time->day = 0;
        out_time->month = 0;
        out_time->year = 0;
        out_time->hour = 0;
        out_time->minute = 0;
        return;
    }

    out_time->day = local_time->tm_mday;
    out_time->month = local_time->tm_mon + 1;
    out_time->year = local_time->tm_year + 1900;
    out_time->hour = local_time->tm_hour;
    out_time->minute = local_time->tm_min;
}
