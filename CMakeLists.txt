cmake_minimum_required(VERSION 3.4)
project(celestial_api)

find_package(jsoncpp REQUIRED)

add_executable(${PROJECT_NAME} src/celestial_api.cpp)
add_subdirectory(curlpp)

target_link_libraries(${PROJECT_NAME} curlpp)
target_link_libraries(${PROJECT_NAME} jsoncpp_lib)
