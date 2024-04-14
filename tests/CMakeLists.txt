add_executable(tests tests.cpp car_park_test.cpp)
target_link_libraries(tests PUBLIC car_park)
target_include_directories(tests PUBLIC ../ext)
target_include_directories(tests PUBLIC ../include)
# allow user to run tests with `make test` or `ctest`
include(../cmake/Catch.cmake)
catch_discover_tests(tests)

set(STATIC_FILES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../static)

# Specify the destination directory where the files will be copied during the build
set(DEST_DIR ${CMAKE_CURRENT_BINARY_DIR}/../static)

# Create a custom command to copy the static files to the build directory
add_custom_command(
        OUTPUT ${DEST_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${STATIC_FILES_DIR} ${DEST_DIR}
        DEPENDS ${STATIC_FILES_DIR}
)

# Add a custom target that depends on the custom command
add_custom_target(copy_static_files ALL DEPENDS ${DEST_DIR})

# Add your executable target and specify that it depends on the copy_static_files target
add_dependencies(tests copy_static_files)