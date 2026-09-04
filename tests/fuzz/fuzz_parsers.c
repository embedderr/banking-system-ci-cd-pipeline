/**
 * @file fuzz_parsers.c
 * @brief libFuzzer harness for ui_screens' internal input-parsing helpers.
 *
 * Targets the exact functions exposed via include/ui_screens_internal.h
 * for unit testing (see docs/design/LLD.md, Section 1.5) -- this harness
 * feeds them semi-random byte input to look for crashes, out-of-bounds
 * reads/writes, or undefined behavior that the hand-written unit tests
 * (UT-104 to UT-120) may not have anticipated.
 *
 * Build (requires clang, Linux):
 *   clang -g -fsanitize=fuzzer,address,undefined -Iinclude \
 *       tests/fuzz/fuzz_parsers.c src/ui_screens.c src/platform.c \
 *       src/address.c src/account.c src/bill_queue.c src/cash_queue.c \
 *       -o fuzz_parsers
 *
 * Run (60 second smoke run, matches the CI job in ci-cd.yml):
 *   ./fuzz_parsers -max_total_time=60
 */

#include "ui_screens_internal.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    /* Fuzzer input is not guaranteed null-terminated; copy into a bounded,
       null-terminated buffer before handing it to functions that expect
       a C string. */
    char buffer[256];
    size_t copy_len = (size < sizeof(buffer) - 1U) ? size : (sizeof(buffer) - 1U);

    memcpy(buffer, data, copy_len);
    buffer[copy_len] = '\0';

    /* --- parse_long --- */
    long long_out;
    (void)parse_long(buffer, &long_out);

    /* --- parse_int --- */
    int int_out;
    (void)parse_int(buffer, &int_out);

    /* --- parse_long_long --- */
    long long ll_out;
    (void)parse_long_long(buffer, &ll_out);

    /* --- parse_double --- */
    double double_out;
    (void)parse_double(buffer, &double_out);

    /* --- copy_bounded: exercise both a buffer that should fit typical
       short input and a deliberately tiny one to stress the boundary
       check itself. */
    char dest_normal[64];
    (void)copy_bounded(dest_normal, sizeof(dest_normal), buffer);

    char dest_tiny[4];
    (void)copy_bounded(dest_tiny, sizeof(dest_tiny), buffer);

    return 0;
}
