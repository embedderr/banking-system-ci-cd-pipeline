#include "unity.h"
#include <stdio.h>

static unsigned int UnityNumberOfTests = 0;
static unsigned int UnityNumberOfFailures = 0;

void UnityBegin(const char* file) {
    printf("\n=== Unity Test Framework Started ===\n");
}

int UnityEnd(void) {
    printf("\n=== Unity Test Summary ===\n");
    printf("Tests Run: %d, Failures: %d\n", UnityNumberOfTests, UnityNumberOfFailures);
    return UnityNumberOfFailures;
}

void UnityDefaultTestRunner(void (*test)(void), const char* name, const char* file, int line) {
    UnityNumberOfTests++;
    printf("RUNNING: %s...\n", name);
    test();
}

void UnityAssertEqualInt(int expected, int actual, const char* file, int line) {
    if (expected != actual) {
        UnityNumberOfFailures++;
        printf("  FAIL: Expected %d but was %d\n", expected, actual);
    } else {
        printf("  PASS\n");
    }
}

void UnityAssertEqualFloat(float expected, float actual, const char* file, int line) {
    if (expected != actual) {
        UnityNumberOfFailures++;
        printf("  FAIL: Expected %.2f but was %.2f\n", expected, actual);
    } else {
        printf("  PASS\n");
    }
}