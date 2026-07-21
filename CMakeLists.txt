cmake_minimum_required(VERSION 3.14)
project(BankingSystem LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Enable coverage for Debug
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --coverage -g -O0")

include_directories(${PROJECT_SOURCE_DIR}/include)

# ---------------------------------------------------------------------------
# Unity Test Framework
#
# Fetched directly from the official ThrowTheSwitch/Unity repository at a
# pinned release tag, rather than hand-vendoring source files. This is the
# standard, reproducible way to consume Unity in a CMake-based C project --
# both local builds and CI resolve the exact same, verified upstream source.
# The previous unity/ directory (a small hand-written stub implementing only
# two assertion macros) has been removed in favor of this.
# ---------------------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG        v2.6.0
)
FetchContent_MakeAvailable(unity)

# Unity disables double-precision assertions (TEST_ASSERT_EQUAL_DOUBLE) by
# default, since it targets embedded platforms that may lack hardware
# double support. Our Money type (common.h) is a double, so this must be
# explicitly enabled for every target that links against unity.
target_compile_definitions(unity PUBLIC UNITY_INCLUDE_DOUBLE)

# Main Application
# NOTE: file(GLOB ...) automatically picks up every .c file placed in
# src/ -- platform.c, address.c, bill_queue.c, cash_queue.c, account.c,
# admin.c, ui_screens.c, and main.c are all included with no changes
# needed here.
file(GLOB SOURCES "src/*.c")
add_executable(banking_system ${SOURCES})

# Unit Tests
#
# The account module's public API was redesigned as part of the C++ to C
# conversion (see docs/design/LLD.md) -- e.g. account_db_insert_pending(),
# account_db_approve(), account_db_apply_withdrawal(), etc. replace the
# previous init_account()/deposit()/withdraw() style API. The previous
# tests/test_account.c (written against the old API) has been removed.
#
# Each module gets its own test executable, linked against the real Unity
# framework fetched above via FetchContent (target name: unity).

add_executable(test_address
    tests/test_address.c
    src/address.c
)
target_link_libraries(test_address unity)
add_test(NAME test_address COMMAND test_address)

add_executable(test_admin
    tests/test_admin.c
    src/admin.c
)
target_link_libraries(test_admin unity)
add_test(NAME test_admin COMMAND test_admin)

add_executable(test_bill_queue
    tests/test_bill_queue.c
    src/bill_queue.c
)
target_link_libraries(test_bill_queue unity)
add_test(NAME test_bill_queue COMMAND test_bill_queue)

add_executable(test_cash_queue
    tests/test_cash_queue.c
    src/cash_queue.c
)
target_link_libraries(test_cash_queue unity)
add_test(NAME test_cash_queue COMMAND test_cash_queue)

add_executable(test_account
    tests/test_account.c
    src/account.c
    src/address.c
)
target_link_libraries(test_account unity)
add_test(NAME test_account COMMAND test_account)

enable_testing()

message(STATUS "Banking System CMake configured successfully")
