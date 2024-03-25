#ifndef CAR_PARK_PROJECT_ORDER_H
#define CAR_PARK_PROJECT_ORDER_H

#include <string>

class Order {
private:
    long long id;
    long long datetime;
    long long driver_id;
    std::string car_number;
    double length;
    double cargo_weight;
    double cost;
public:
    Order();
    explicit Order(std::string sql_data);
    double getCargoWeight() {
        return cargo_weight;
    }
    std::string getCarNumber() {
        return car_number;
    }
    long long getId() {
        return id;
    }
    long long getDatetime() {
        return datetime;
    }
    long long getDriverId() {
        return driver_id;
    }
    double getLength() {
        return length;
    }
    double getCost() {
        return cost;
    }
};


#endif //CAR_PARK_PROJECT_ORDER_H
