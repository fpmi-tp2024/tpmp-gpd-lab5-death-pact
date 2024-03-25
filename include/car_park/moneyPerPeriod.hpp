
#ifndef CAR_PARK_PROJECT_MONEYPERPERIOD_H
#define CAR_PARK_PROJECT_MONEYPERPERIOD_H

#pragma once
#include <string>
class MoneyPerPeriod {
private:
    long long id;
    long long datetime_from;
    long long datetime_to;
    long long driver_id;
    long double total_money;
public:
    MoneyPerPeriod();
    MoneyPerPeriod(std::string sql_data);
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


#endif //CAR_PARK_PROJECT_MONEYPERPERIOD_H
