cmake_minimum_required(VERSION 3.8)
project(ceeds)

set(CMAKE_C_STANDARD 11)

set(PREPROCESSOR_FLAGS "-D_GNU_SOURCE -D__FILENAME__=\"$(<F)\"")
set(WARNING_FLAGS "-Wall -Wextra -Wundef -Wshadow -Wpointer-arith")
set(OPTIMIZATION_FLAGS "-O3")

set(CMAKE_C_FLAGS "${PREPROCESSOR_FLAGS} ${WARNING_FLAGS} ${OPTIMIZATION_FLAGS}")

add_library(ceeds INTERFACE)

target_sources(ceeds INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ceeds/ascii_set.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ceeds/bitmanip.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ceeds/core.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ceeds/list.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ceeds/memory.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ceeds/memory_allocator.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ceeds/string_utils.h

        ${CMAKE_CURRENT_SOURCE_DIR}/src/memory.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/string_utils.c
        )

target_include_directories(ceeds INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

option(CEEDS_BUILD_TESTS "Build tests of the ceeds library" ON)

if (CEEDS_BUILD_TESTS)
    add_executable(ceeds-tests
            tests/unit_tests.h
            tests/ascii_set-tests.c
            tests/bitmanip-tests.c
            tests/core-tests.c
            tests/list-tests.c
            tests/memory-tests.c
            tests/string_utils-tests.c
            tests/main.c
            )

    target_link_libraries(ceeds-tests PRIVATE ceeds)
endif ()
