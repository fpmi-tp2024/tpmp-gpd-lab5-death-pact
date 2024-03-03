add_executable(tests tests.cpp car_park_test.cpp)
target_link_libraries(tests PUBLIC car_park)
target_include_directories(tests PUBLIC ../ext)

# allow user to run tests with `make test` or `ctest`
include(../cmake/Catch.cmake)
catch_discover_tests(tests)