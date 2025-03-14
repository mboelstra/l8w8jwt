#  Copyright 2020 Raphael Beck
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

cmake_minimum_required(VERSION 3.1)
project(l8w8jwt C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(L8W8JWT_MAJOR 1)
set(L8W8JWT_MINOR 0)
set(L8W8JWT_PATCH 0)
set(L8W8JWT_VERSION_STRING "${L8W8JWT_MAJOR}.${L8W8JWT_MINOR}.${L8W8JWT_PATCH}")

option(L8W8JWT_ENABLE_TESTS "Build l8w8jwt tests." OFF)
option(L8W8JWT_ENABLE_EXAMPLES "Build l8w8jwt example programs." OFF)
option(L8W8JWT_PACKAGE "Build the library and package it into a .tar.gz after successfully building." OFF)

option(L8W8JWT_DLL "Use l8w8jwt as a DLL." OFF)
option(L8W8JWT_BUILD_DLL "Build l8w8jwt as a DLL." OFF)

if (L8W8JWT_BUILD_DLL)
    add_compile_definitions("L8W8JWT_BUILD_DLL=1")
    set(L8W8JWT_DLL ON)
endif ()

if (L8W8JWT_DLL)
    add_compile_definitions("L8W8JWT_DLL=1")
endif ()

option(ENABLE_TESTING "Build MbedTLS tests." OFF)
option(ENABLE_PROGRAMS "Build MbedTLS example programs." OFF)

if (NOT TARGET mbedtls)
    add_subdirectory(lib/mbedtls mbedtls)
endif ()

if (NOT TARGET chillbuff)
    add_subdirectory(lib/chillbuff chillbuff)
endif ()

set(l8w8jwt_headers
        ${CMAKE_CURRENT_LIST_DIR}/lib/jsmn/jsmn.h
        ${CMAKE_CURRENT_LIST_DIR}/lib/checknum/include/checknum.h
        ${CMAKE_CURRENT_LIST_DIR}/include/l8w8jwt/retcodes.h
        ${CMAKE_CURRENT_LIST_DIR}/include/l8w8jwt/algs.h
        ${CMAKE_CURRENT_LIST_DIR}/include/l8w8jwt/base64.h
        ${CMAKE_CURRENT_LIST_DIR}/include/l8w8jwt/claim.h
        ${CMAKE_CURRENT_LIST_DIR}/include/l8w8jwt/encode.h
        ${CMAKE_CURRENT_LIST_DIR}/include/l8w8jwt/decode.h
        ${CMAKE_CURRENT_LIST_DIR}/include/l8w8jwt/version.h
        )

set(l8w8jwt_sources
        ${CMAKE_CURRENT_LIST_DIR}/src/base64.c
        ${CMAKE_CURRENT_LIST_DIR}/src/claim.c
        ${CMAKE_CURRENT_LIST_DIR}/src/encode.c
        ${CMAKE_CURRENT_LIST_DIR}/src/decode.c
        )

add_library(${PROJECT_NAME} ${l8w8jwt_headers} ${l8w8jwt_sources})

if (L8W8JWT_BUILD_DLL OR BUILD_SHARED_LIBS)
    set_property(TARGET mbedtls PROPERTY POSITION_INDEPENDENT_CODE ON)
    set_property(TARGET mbedx509 PROPERTY POSITION_INDEPENDENT_CODE ON)
    set_property(TARGET mbedcrypto PROPERTY POSITION_INDEPENDENT_CODE ON)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            VERSION ${L8W8JWT_VERSION_STRING}
            SOVERSION ${L8W8JWT_MAJOR}
            )
endif ()

if (L8W8JWT_PACKAGE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/bin"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/bin"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/bin"

            ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/bin/debug"
            LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/bin/debug"
            RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/bin/debug"

            ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/bin/release"
            LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/bin/release"
            RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/bin/release"
            )

    add_custom_command(TARGET ${PROJECT_NAME}
            POST_BUILD COMMAND ${CMAKE_COMMAND}
            -E copy ${CMAKE_CURRENT_LIST_DIR}/NOTICE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/NOTICE"
            )

    add_custom_command(TARGET ${PROJECT_NAME}
            POST_BUILD COMMAND ${CMAKE_COMMAND}
            -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/include "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/include"
            )

    string(TOLOWER "${PROJECT_NAME}-${L8W8JWT_VERSION_STRING}-${CMAKE_SYSTEM_NAME}-${CMAKE_HOST_SYSTEM_PROCESSOR}.tar.gz" L8W8JWT_OUTPUT_ARCHIVE_FILENAME)

    add_custom_command(TARGET ${PROJECT_NAME}
            POST_BUILD COMMAND ${CMAKE_COMMAND}
            -E tar "czvf" ${L8W8JWT_OUTPUT_ARCHIVE_FILENAME} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
            )
endif ()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lib/checknum/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lib/mbedtls/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lib/jsmn)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(${PROJECT_NAME} PUBLIC chillbuff)
target_link_libraries(${PROJECT_NAME} PRIVATE mbedtls mbedx509 mbedcrypto chillbuff)

if (L8W8JWT_ENABLE_EXAMPLES)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/examples)
endif ()

if (L8W8JWT_ENABLE_TESTS)

    if (WIN32 AND BUILD_SHARED_LIBS)
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    endif ()

    set(TESTS
            ${CMAKE_CURRENT_LIST_DIR}/tests/testkeys.h
            ${CMAKE_CURRENT_LIST_DIR}/tests/tests.c
            )

    add_executable(run_tests ${TESTS})

    target_include_directories(run_tests PUBLIC ${CMAKE_CURRENT_LIST_DIR}/lib/acutest/include)

    target_link_libraries(run_tests PUBLIC ${PROJECT_NAME})

    if (ENABLE_COVERAGE)
        if (L8W8JWT_ENABLE_EXAMPLES)
            message(FATAL_ERROR "ENABLE_COVERAGE and L8W8JWT_ENABLE_EXAMPLES are MUTUALLY EXCLUSIVE! Please enable either one or the other, but not both at the same time!")
        else ()
            find_package(codecov)
            add_coverage(l8w8jwt)
            add_coverage(run_tests)
            coverage_evaluate()
        endif ()
    endif ()
endif ()
