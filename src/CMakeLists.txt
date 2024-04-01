add_library(car_park car_park.cpp user.cpp driver.cpp car.cpp  order.cpp  moneyPerPeriod.cpp)
add_library(sqlite3 ../include/sql/sqlite3.c)
target_include_directories(car_park PUBLIC ../include)
target_link_libraries(car_park PUBLIC sqlite)
# require at least c++ 20
target_compile_features(car_park PUBLIC cxx_std_20)
