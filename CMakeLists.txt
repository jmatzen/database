cmake_minimum_required (VERSION 3.7)
project(database)

add_compile_options(/std:c++latest)
set(BOOST_DIR ${CMAKE_SOURCE_DIR}/../boost_1_64_0)
add_definitions(-D_WIN32_WINNT=0x0601 -D_CRT_SECURE_NO_WARNINGS)
include_directories(${BOOST_DIR})
link_directories(${BOOST_DIR}/lib64-msvc-14.1)


add_subdirectory(src)

