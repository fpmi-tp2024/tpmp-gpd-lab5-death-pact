
#ifndef CAR_PARK_PROJECT_MONEYPERPERIOD_H
#define CAR_PARK_PROJECT_MONEYPERPERIOD_H

#pragma once
#include <string>
#include <vector>
#include "car_park/user.hpp"
#include "car_park/driver.hpp"

namespace car_park {
    class MoneyPerPeriod {
    private:
        MoneyPerPeriod();
        static long long next_id;
        long long id;
        long long datetime_from;
        long long datetime_to;
        long long driver_id;
        long double total_money;
    public:


        explicit MoneyPerPeriod(std::string sql_data);

        long long getId() {
            return id;
        }

        long long getDatetimeFrom() {
            return datetime_from;
        }

        long long getDatetimeTo() {
            return datetime_to;
        }

        long long getDriverId() {
            return driver_id;
        }

        long double getTotalMoney() {
            return total_money;
        }
    };

    class MoneyPerPeriodDAO {
    private:
        static bool insert(MoneyPerPeriod* mpp);
    public:
        static MoneyPerPeriod* count(User& user, Driver& driver, long long datetime_start, long long datetime_end);
        static std::vector<MoneyPerPeriod*> count_all(User& user, long long datetime_start, long long datetime_end);
    };
}

#endif //CAR_PARK_PROJECT_MONEYPERPERIOD_H
