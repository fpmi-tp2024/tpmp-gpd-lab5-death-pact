add_library(car_park car_park.cpp)
target_include_directories(car_park PUBLIC ../include)
# require at least c++ 20
target_compile_features(car_park PUBLIC cxx_std_20)
