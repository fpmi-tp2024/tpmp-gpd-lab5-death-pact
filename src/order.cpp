#include <string>
#include "car_park/order.hpp"
#include "sql/sqlite3.h"
#include <sstream>

namespace car_park {

    Order::Order(std::string sql_data) {
        std::stringstream ss(sql_data);
        ss >> id;
        ss.ignore();
        ss >> datetime;
        ss.ignore();
        ss >> driver_id;
        ss.ignore();
        std::getline(ss, car_number, ',');
        ss >> length;
        ss.ignore();
        ss >> cargo_weight;
        ss.ignore();
        ss >> cost;
    }


    static int f_select(void *ordersListPtr, int argc, char **argv, char **azColName){
        if (argc == 0) return 0;

        auto orders = (std::vector<Order>*)ordersListPtr;
        std::string order_data = std::string(argv[0] ? argv[0] : "NULL");
        for (int i = 1; i < argc; i++) {
            order_data += "," + std::string(argv[i] ? argv[i] : "NULL");
        }
        orders->emplace_back(Order(order_data));
        return 0;
    }

    bool OrdersDAO::check_weight(Order* order, Car* car){                                      // To fix: encapsulation violations
        return order->getCargoWeight() <= car->getCapacity();
    }

    void OrdersDAO::find_all_by_driver(Driver& driver, std::vector<Order>& orders_of_driver){
        sqlite3 *db;
        char *zErrMsg = nullptr;
        std::vector<Order>* ordersListPtr = &orders_of_driver;
        const char *sql;
        int rc;
        rc = sqlite3_open("../ext/autopark.db", &db);
        std::string sql_str = "SELECT * FROM orders WHERE driver_id = " + std::to_string(driver.getId()) + ";";
        sql = sql_str.c_str();
        rc = sqlite3_exec(db, sql, f_select, (void*)ordersListPtr, &zErrMsg);
        sqlite3_close(db);
    }
    bool OrdersDAO::insert(Order& order){
        if (OrdersDAO::check_weight(&order, CarsDAO::find_by_number(User(), order.getCarNumber())))
        {
            sqlite3 *db;
            char *zErrMsg = nullptr;
            char *sql_data;
            const char *sql;
            int rc;
            rc = sqlite3_open("../ext/autopark.db", &db);
            std::string sql_str = "INSERT INTO orders (id,datetime,driver_id,car_number,length,cargo_weight,cost) "  \
                            "VALUES (" + std::to_string(order.getId()) + ", " + std::to_string(order.getDatetime()) + ", " + std::to_string(order.getDriverId()) + ", '"
                                  + order.getCarNumber() + "', " + std::to_string(order.getLength()) + ", " + std::to_string(order.getCargoWeight()) +
                                  ", " + std::to_string(order.getCost()) + "); ";
            sql = sql_str.c_str();
            rc = sqlite3_exec(db, sql, nullptr, (void*)sql_data, &zErrMsg);
            sqlite3_close(db);
            return true;
        }
        return false;
    }
    void OrdersDAO::update(Order& order){
        sqlite3 *db;
        char *zErrMsg = nullptr;
        char *sql_data;
        const char *sql;
        int rc;
        rc = sqlite3_open("../ext/autopark.db", &db);
        std::string sql_str = "UPDATE orders" \
                          "SET datetime = " + std::to_string(order.getDatetime()) + ","  \
                          "driver_id = " + std::to_string(order.getDriverId()) + ","  \
                          "car_number = " + order.getCarNumber() + ","  \
                          "length = " + std::to_string(order.getLength()) + ","  \
                          "cargo_weight = " + std::to_string(order.getCargoWeight()) + ","  \
                          "cost = " + std::to_string(order.getCost()) + " "  \
                          "WHERE id = " + std::to_string(order.getId()) + "; ";
        sql = sql_str.c_str();
        rc = sqlite3_exec(db, sql, nullptr, (void*)sql_data, &zErrMsg);
        sqlite3_close(db);
    }
}