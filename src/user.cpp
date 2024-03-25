#include "car_park/user.hpp"
#include <cstring>
#include <sstream>
User::User() {
    login = "";
    type = "superuser";
}

User::User(std::string sql_data) {
    const char* delim = ",";
    char* value = strtok(sql_data.data(), delim);
    login = std::string(value);
    value = strtok(nullptr, delim);
    type = std::string(value);
}

std::string User::get_login() {
    return login;
}

std::string User::get_type() {
    return type;
}

