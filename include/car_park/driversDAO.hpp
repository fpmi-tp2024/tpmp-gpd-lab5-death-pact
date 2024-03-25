#ifndef CAR_PARK_PROJECT_DRIVERSDAO_H
#define CAR_PARK_PROJECT_DRIVERSDAO_H

#pragma once
#include <string>
#include <vector>
#include "car_park/driver.hpp"
#include "car_park/user.hpp"
class DriversDAO {
public:
    static Driver* find_by_user(User& user);
    static Driver* find_with_min_orders(User& user);
    static void find_all(User& user, std::vector<Driver>& drivers);
    static void insert(Driver& driver);
};


#endif //CAR_PARK_PROJECT_DRIVERSDAO_H
