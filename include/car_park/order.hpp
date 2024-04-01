#ifndef CAR_PARK_PROJECT_ORDER_H
#define CAR_PARK_PROJECT_ORDER_H

#include <string>
#include <vector>
#include "car_park/driver.hpp"
#include "car_park/car.hpp"

namespace car_park {
    class Order {
    private:
        Order();
        long long id;
        long long datetime;
        long long driver_id;
        std::string car_number;
        double length;
        double cargo_weight;
        double cost;
    public:


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

    class OrdersDAO {
    public:
        static bool check_weight(Order* order, Car* car);
        static void find_all_by_driver(Driver& driver, std::vector<Order>& orders_of_driver);
        static bool insert(Order& order);
        static void update(Order& order);

    };
}

#endif //CAR_PARK_PROJECT_ORDER_H
