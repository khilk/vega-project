cmake_minimum_required(VERSION 3.23)

project(raytracer)

set(RT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(RT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

# Include guards
if (RT_SOURCE_DIR STREQUAL RT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif ()

# Set variables
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_VISIBILITY_PRESET hidden)

set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
    list(APPEND RT_COMPILE_OPTIONS /W3)
    # TODO We have to use /Wall. But MSVC is dumb and uses this flag for external projects, like gtest.
    if (RT_SANITIZER)
        message("Sanitizer's work only on Linux")
    endif ()
else ()
    list(APPEND RT_COMPILE_OPTIONS -Wall -Wextra -pedantic)
    if (RT_SANITIZER STREQUAL OFF)
        message("All sanitizers disabled")
    elseif (RT_SANITIZER STREQUAL ASAN)
        set(SANITIZER_ENABLE_OPTION -fsanitize=address)
        set(SANITIZER_COMPILE_OPTIONS
                # -fsanitize-address-use-after-return=runtime
                -fsanitize-recover=address
                -fsanitize-address-use-after-scope
                )
    elseif (RT_SANITIZER STREQUAL TSAN)
        set(SANITIZER_ENABLE_OPTION -fsanitize=thread)
        set(SANITIZER_COMPILE_OPTIONS
                -gcolumn-info
                )
    elseif (RT_SANITIZER STREQUAL UBSAN)
        set(SANITIZER_ENABLE_OPTION -fsanitize=undefined)
    elseif (RT_SANITIZER STREQUAL MEMSAN) # Memory Santizer doesn't work with GTest (https://github.com/google/sanitizers/issues/648)
        set(SANITIZER_ENABLE_OPTION -fsanitize=memory)
    endif ()
    list(APPEND RT_COMPILE_OPTIONS
            -fno-omit-frame-pointer
            ${SANITIZER_ENABLE_OPTION}
            ${SANITIZER_COMPILE_OPTIONS}
            )
    list(APPEND RT_LINK_OPTIONS ${SANITIZER_ENABLE_OPTION})
endif ()

if (RT_COVERAGE AND CMAKE_SYSTEM_NAME STREQUAL Linux)
    list(APPEND RT_COMPILE_OPTIONS
            --coverage
            )
    list(APPEND RT_LINK_OPTIONS
            --coverage
            )
endif ()
message("RT_COMPILE_OPTIONS: ${RT_COMPILE_OPTIONS}")
message("RT_LINK_OPTIONS   : ${RT_LINK_OPTIONS}")
add_definitions(${RT_DEFINITIONS})

# Main library directory
add_subdirectory(src)

if (BUILD_TESTING)
    include(FetchContent)
    enable_testing()
    add_subdirectory(test)
endif ()
