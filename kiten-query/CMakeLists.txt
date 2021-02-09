set(CURRENT_TARGET "kiten_query")
set(CURRENT_TARGET_NAME "kiten-query")
set(CURRENT_TARGET_INTERFACE "${CURRENT_TARGET}_interface")

message(STATUS "Configuring ${CURRENT_TARGET}...")

# Qt
find_package(Qt5Core REQUIRED)

# reproc
message(STATUS "configure reproc...")
set(REPROC++ ON)
set(REPROC_TEST OFF)
set(REPROC_EXAMPLES OFF)
set(REPROC_OBJECT_LIBRARIES ON)
set(REPROC_INSTALL OFF)
set(REPROC_DEVELOP OFF)
set(REPROC_SANITIZERS OFF)
set(REPROC_TIDY OFF)
set(REPROC_WARNINGS OFF)
set(REPROC_WARNINGS_AS_ERRORS OFF)
add_subdirectory(reproc)
# required to build with extensions disabled
target_compile_definitions(reproc PRIVATE "-D_GNU_SOURCE")
message(STATUS "configured reproc.")

# create targets after qt setup
add_executable(kiten-query-helper "query-helper.cpp")
set_target_properties(kiten-query-helper PROPERTIES PREFIX "")
set_target_properties(kiten-query-helper PROPERTIES OUTPUT_NAME "kiten-query-helper")
SetCppStandard(kiten-query-helper 17)
set_target_properties(kiten-query-helper PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(kiten-query-helper PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(kiten-query "main.cpp")
set_target_properties(kiten-query PROPERTIES PREFIX "")
set_target_properties(kiten-query PROPERTIES OUTPUT_NAME "kiten-query")
SetCppStandard(kiten-query 17)
set_target_properties(kiten-query PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(kiten-query PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(kiten-query PRIVATE "-DCMAKE_INSTALL_PREFIX=\"${CMAKE_INSTALL_PREFIX}\"")

# Qt deprecated warnings
target_compile_definitions(kiten-query-helper PRIVATE -DQT_DEPRECATED_WARNINGS)
target_compile_definitions(kiten-query-helper PRIVATE -DQT_DISABLE_DEPRECATED_BEFORE=0x060000)

# disable Qt foreach macro
target_compile_definitions(kiten-query-helper PRIVATE -DQT_NO_FOREACH)

target_link_libraries(kiten-query-helper
    PRIVATE
        Qt5::Core
        -lkiten
)

target_link_libraries(kiten-query PRIVATE reproc++)

install(TARGETS kiten-query DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
install(TARGETS kiten-query-helper DESTINATION "${CMAKE_INSTALL_PREFIX}/libexec")

message(STATUS "Configured ${CURRENT_TARGET}.")
