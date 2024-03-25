#include <cstring>
#include "car_park/driver.hpp"
#include <cstdlib>
Driver::Driver() {

}

Driver::Driver(std::string sql_data) {
    const char* delim = ",";
    char *endptr;
    char* value = strtok(sql_data.data(), delim);
    id = strtoll(value, &endptr, 10);
    value = strtok(nullptr, delim);
    user_login = std::string(value);
    value = strtok(nullptr, delim);
    name = std::string(value);
    value = strtok(nullptr, delim);
    category = std::string(value);
    value = strtok(nullptr, delim);
    career_start = strtoll(value, &endptr, 10);
    value = strtok(nullptr, delim);
    birth_year = strtoll(value, &endptr, 10);
}

int Driver::count_order_per_period(long long start_date, long long end_date) {

}