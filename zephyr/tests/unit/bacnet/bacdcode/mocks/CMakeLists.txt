#
# CMakeLists.txt file for creating mocks library.
#

add_library(mocks STATIC
  ${ZEPHYR_CURRENT_MODULE_DIR}/test/unit/bacnet/bacdcode/src/mocks/bacint.c
  ${ZEPHYR_CURRENT_MODULE_DIR}/test/unit/bacnet/bacdcode/src/mocks/bacreal.c
  ${ZEPHYR_CURRENT_MODULE_DIR}/test/unit/bacnet/bacdcode/src/mocks/bacstr.c
  )

target_include_directories(mocks PUBLIC
  ${ZEPHYR_CURRENT_MODULE_DIR}/src
  ${ZEPHYR_CURRENT_MODULE_DIR}/test/unit/bacnet/bacdcode/src
  )

target_link_libraries(mocks PRIVATE test_interface)
