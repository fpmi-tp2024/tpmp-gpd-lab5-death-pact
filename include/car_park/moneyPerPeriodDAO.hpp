
#ifndef CAR_PARK_PROJECT_MONEYPERPERIODDAO_H
#define CAR_PARK_PROJECT_MONEYPERPERIODDAO_H


#pragma once
#include <string>
#include <vector>
#include "car_park/moneyPerPeriod.hpp"
#include "car_park/user.hpp"
#include "car_park/driver.hpp"


class MoneyPerPeriodDAO {
public:
    static MoneyPerPeriod* count(User& user, Driver& driver, long long datetime_start, long long datetime_end);
    static std::vector<MoneyPerPeriod*> count_all(User& user, long long datetime_start, long long datetime_end);
};


#endif //CAR_PARK_PROJECT_MONEYPERPERIODDAO_H
