cmake_minimum_required(VERSION 3.10)
project(BankingSystem LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Enable coverage for Debug
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --coverage -g -O0")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/unity)

# Main Application
file(GLOB SOURCES "src/*.c")
add_executable(banking_system ${SOURCES})

# Unit Test
add_executable(test_account 
    tests/test_account.c 
    src/account.c 
    unity/unity.c
)

add_test(NAME test_account COMMAND test_account)

enable_testing()

message(STATUS "Banking System CMake configured successfully")