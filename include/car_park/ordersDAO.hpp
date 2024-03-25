#ifndef CAR_PARK_PROJECT_ORDERSDAO_H
#define CAR_PARK_PROJECT_ORDERSDAO_H


#pragma once
#include <string>
#include <vector>
#include "car_park/order.hpp"
#include "car_park/driver.hpp"
#include "car_park/car.hpp"

class OrdersDAO {
public:
    static bool check_weight(Order* order, Car* car);
    static void find_all_by_driver(Driver& driver, std::vector<Order>& orders_of_driver);
    static bool insert(Order& order);
    static void update(Order& order);

};


#endif //CAR_PARK_PROJECT_ORDERSDAO_H
