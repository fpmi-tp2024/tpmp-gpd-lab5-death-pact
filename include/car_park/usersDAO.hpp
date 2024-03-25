#ifndef CAR_PARK_PROJECT_USERSDAO_H
#define CAR_PARK_PROJECT_USERSDAO_H

#pragma once
#include <string>
#include "car_park/user.hpp"

class UsersDAO {
public:
    static User* find(std::string login, std::string password);
    static void insert(User& user, std::string password);
    static void update(User& user, std::string password);
};

#endif //CAR_PARK_PROJECT_USERSDAO_H
