#ifndef CAR_PARK_PROJECT_USER_H
#define CAR_PARK_PROJECT_USER_H

#pragma once
#include <string>

class User {
private:
    std::string login;
    std::string type;
public:
    User();
    User(std::string sql_data);
    std::string get_login();
    std::string get_type();
};

#endif //CAR_PARK_PROJECT_USER_H
