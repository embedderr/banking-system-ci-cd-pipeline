/**
 * @file ui_screens_internal.h
 * @brief Test-only visibility into ui_screens' internal input-parsing
 *        helpers.
 *
 * These functions are part of ui_screens.c's implementation, not its
 * public API (see ui_screens.h for the real module interface used by
 * main.c). They are declared here, non-static, SOLELY so that
 * tests/test_ui_screens_internal.c can exercise them directly and close
 * the SRS-027 coverage gap noted in traceability/RTM.xlsx.
 *
 * No production code should include this header. main.c and every other
 * module continue to interact with ui_screens exclusively through
 * ui_screens.h's public collection/display functions.
 */

#ifndef UI_SCREENS_INTERNAL_H
#define UI_SCREENS_INTERNAL_H

#include <stdbool.h>
#include <stddef.h>

bool read_line(char *buffer, size_t size);
bool parse_long(const char *str, long *out);
bool parse_int(const char *str, int *out);
bool parse_long_long(const char *str, long long *out);
bool parse_double(const char *str, double *out);
bool copy_bounded(char *dest, size_t dest_size, const char *src);

#endif /* UI_SCREENS_INTERNAL_H */
