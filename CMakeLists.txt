cmake_minimum_required (VERSION 3.5)
project(database)


if (APPLE)
  add_compile_options(-std=c++1z)
  set(BOOST_DIR ${CMAKE_SOURCE_DIR}/../boost-build)
  link_directories(${BOOST_DIR}/lib)
  include_directories(${BOOST_DIR}/include)
else()
  add_compile_options(/std:c++latest)
  set(BOOST_DIR ${CMAKE_SOURCE_DIR}/../boost_1_64_0)
  add_definitions(-D_WIN32_WINNT=0x0601 -D_CRT_SECURE_NO_WARNINGS)
  link_directories(${BOOST_DIR}/lib64-msvc-14.1)
  include_directories(${BOOST_DIR})
endif()



add_subdirectory(src)

