cmake_minimum_required(VERSION 3.10)

option(build "Build code." ON)
option(ut "Build all tests." ON)

project(Proj)

set(CMAKE_CXX_FLAGS "-g -Wall -Werror -Wno-missing-braces")
set(CMAKE_CXX_STANDARD 17)

set(PROJECT_IMPL src/impl)
file(GLOB_RECURSE sources
  ${PROJECT_IMPL}/*.cpp
)

if (build)
  include_directories(
    ${PROJECT_IMPL}
  )
  add_executable(build
    src/main.cpp
    ${sources}
  )
endif()

if (ut)
  add_library(impl ${sources})
  add_subdirectory(3rdparty/gtest-1.8.0)
  enable_testing()
  include_directories(
    ${gtest_SOURCE_DIR}/include
    ${gtest_SOURCE_DIR}
  )

  set(TEST_DIR tests/)
  file(GLOB_RECURSE test_sources ${TEST_DIR}/*.cpp)
  add_executable(ut ${test_sources})

  target_link_libraries(ut
    gtest
    gtest_main
    impl
  )

  add_test(NAME sum COMMAND ut)
endif()
