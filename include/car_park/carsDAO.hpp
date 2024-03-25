#ifndef CAR_PARK_PROJECT_CARSDAO_H
#define CAR_PARK_PROJECT_CARSDAO_H

#pragma once
#include "car_park/car.hpp"
#include "car_park/user.hpp"
#include "car_park/driver.hpp"
#include <string>


class CarsDAO {
public:
    static void insert(Car& car);
    static Car* find_by_number(User& user, std::string car_number);
    static Car* find_with_max_total_mileage();
};


#endif //CAR_PARK_PROJECT_CARSDAO_H
