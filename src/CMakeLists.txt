file(GLOB files *.h *.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/rapidjson/include)
add_executable(database ${files})
if (APPLE)
	target_link_libraries(database boost_system boost_thread)
endif()

