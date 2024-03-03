cmake_minimum_required(VERSION 3.23)
project(car_park_project)

include(CTest)

set(CMAKE_CXX_STANDARD 20)

# compile the library
add_subdirectory(src)

# compile the tests
add_subdirectory(tests)

# compile the application
add_subdirectory(app)
