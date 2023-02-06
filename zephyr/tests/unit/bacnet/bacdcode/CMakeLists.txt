# Copyright (c) 2022 Legrand North America, LLC.
#
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

if(NOT ZEPHYR_CURRENT_MODULE_DIR)
  string(REGEX REPLACE "/zephyr/tests/[a-zA-Z_/-]*$" ""
    ZEPHYR_CURRENT_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# NOTE for Zephyr >= v3.2.0:
# - Zephyr unittest does not generate ZEPHYR_<modulename>_MODULE_DIR.
# So we have to use relative paths to get to the source.

set(SOURCES
  ${ZEPHYR_CURRENT_MODULE_DIR}/test/unit/bacnet/bacdcode/src/main.c
  )

project(bacnet_bacdcode)
find_package(Zephyr COMPONENTS unittest REQUIRED HINTS $ENV{ZEPHYR_BASE})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mocks)

target_include_directories(testbinary PRIVATE
  ${ZEPHYR_CURRENT_MODULE_DIR}/src
  mocks
  )

target_link_libraries(testbinary PRIVATE mocks)
