cmake_minimum_required(VERSION 3.14)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

project(kiten-krunner C CXX)
include(ProjectSetup)

# krunner plugin
add_subdirectory(plugin)

# kiten-query command
add_subdirectory(kiten-query)
