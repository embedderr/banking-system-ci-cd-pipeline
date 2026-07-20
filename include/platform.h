/**
 * @file platform.h
 * @brief Portable interface for every operation that differs by operating
 *        system (clearing the screen, waiting for a keypress, reading the
 *        current time).
 *
 * This is the ONLY module in the system permitted to include OS-specific
 * headers or call OS-specific functions. Every other module must go
 * through the functions declared here instead of calling the OS directly.
 *
 * Traceability: satisfies SRS-025 (system implemented in standard C,
 * core business-logic modules must not depend on any OS-specific API).
 */

#ifndef PLATFORM_H
#define PLATFORM_H

/**
 * @brief Portable representation of a point in time.
 *
 * Filled by platform_get_current_time() so that no other module needs to
 * include <time.h> or call time()/localtime() directly.
 */
typedef struct {
    int day;    /**< Day of month, 1-31. */
    int month;  /**< Month, 1-12. */
    int year;   /**< Full year, e.g. 2026. */
    int hour;   /**< Hour, 0-23. */
    int minute; /**< Minute, 0-59. */
} SystemTime;

/**
 * @brief Clears the console/terminal screen.
 *
 * Windows builds wrap system("cls"); a Linux/macOS build would wrap
 * system("clear") instead — this is the only function that needs to
 * change to port the screen-clearing behavior to a new OS.
 */
void platform_clear_screen(void);

/**
 * @brief Blocks until the user presses a single key, without requiring
 *        Enter to be pressed and without echoing the key to the screen.
 *
 * Windows builds wrap _getch(); a Linux/macOS build would use a
 * termios-based raw-mode read instead.
 */
void platform_wait_for_keypress(void);

/**
 * @brief Fills @p out_time with the current local date and time.
 *
 * @param out_time  Non-NULL pointer to the SystemTime struct to fill.
 *                  If NULL, the function returns immediately without
 *                  writing anything.
 */
void platform_get_current_time(SystemTime *out_time);

#endif /* PLATFORM_H */
