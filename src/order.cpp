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

    bool OrdersDAO::check_weight(Order* order, Car* car){
        if (order == nullptr || car == nullptr)
            return false;
        return order->getCargoWeight() <= car->getCapacity();
    }

    void OrdersDAO::count_order_per_period(car_park::Driver &driver,
                                           std::vector<Order> &orders_of_driver, long long start_date,
                                           long long end_date) {
        sqlite3 *db;
        int rc = sqlite3_open("../autopark.db", &db);
        if (rc != SQLITE_OK)
            return;
        std::string sql = "SELECT id,datetime,driver_id,car_number,length,cargo_weight,cost FROM orders WHERE driver_id = ? AND datetime >= ? AND datetime <= ?";
        sqlite3_stmt *stmt;
        int count = 0;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int64(stmt, 1, driver.getId());
            sqlite3_bind_int64(stmt, 2, start_date);
            sqlite3_bind_int64(stmt, 3, end_date);

            while (sqlite3_step(stmt) == SQLITE_ROW) {
                int id = sqlite3_column_int(stmt, 0);
                int datetime = sqlite3_column_int(stmt, 1);
                int driver_id = sqlite3_column_int(stmt, 2);
                std::string car_number = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                double length = sqlite3_column_double(stmt, 4);
                double cargo_weight = sqlite3_column_double(stmt, 5);
                double cost = sqlite3_column_double(stmt, 6);

                orders_of_driver.emplace_back(Order(std::to_string(id) + "," + std::to_string(datetime) + "," + std::to_string(driver_id) + ","  +
                                                    car_number + "," + std::to_string(length) + "," + std::to_string(cargo_weight) + "," + std::to_string(cost)));
            }
            sqlite3_finalize(stmt);
        }
        sqlite3_close(db);
    }

    void OrdersDAO::find_all_by_driver(User& user, Driver& driver, std::vector<Order>& orders_of_driver){
        if (!User::check_access(user))
            return;

        sqlite3 *db;
        int rc = sqlite3_open("../autopark.db", &db);
        if (rc != SQLITE_OK)
            return;
        std::string sql = "SELECT id,datetime,driver_id,car_number,length,cargo_weight,cost FROM orders WHERE driver_id = ?";
        sqlite3_stmt* stmt;

        if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int64(stmt, 1, driver.getId());

            while (sqlite3_step(stmt) == SQLITE_ROW) {
                int id = sqlite3_column_int(stmt, 0);
                int datetime = sqlite3_column_int(stmt, 1);
                int driver_id = sqlite3_column_int(stmt, 2);
                std::string car_number = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                double length = sqlite3_column_double(stmt, 4);
                double cargo_weight = sqlite3_column_double(stmt, 5);
                double cost = sqlite3_column_double(stmt, 6);

                orders_of_driver.emplace_back(Order(std::to_string(id) + "," + std::to_string(datetime) + "," + std::to_string(driver_id) + ","  +
                                                    car_number + "," + std::to_string(length) + "," + std::to_string(cargo_weight) + "," + std::to_string(cost)));
            }

            sqlite3_finalize(stmt);
        }
        sqlite3_close(db);
    }
    bool OrdersDAO::insert(User& user, Order& order) {
        if (!User::check_access(user))
            return false;

        if (OrdersDAO::check_weight(&order, CarsDAO::find_by_number(user, order.getCarNumber()))) {
            sqlite3 *db;
            int rc = sqlite3_open("../autopark.db", &db);
            if (rc != SQLITE_OK)
                return false;
            std::string sql = "INSERT INTO orders (datetime, driver_id, car_number, length, cargo_weight, cost) VALUES (?, ?, ?, ?, ?, ?)";
            sqlite3_stmt *stmt;
            if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int64(stmt, 1, order.getDatetime());
                sqlite3_bind_int64(stmt, 2, order.getDriverId());
                sqlite3_bind_text(stmt, 3, order.getCarNumber().c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_double(stmt, 4, order.getLength());
                sqlite3_bind_double(stmt, 5, order.getCargoWeight());
                sqlite3_bind_double(stmt, 6, order.getCost());
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            } else {
                return false;
            }

            sqlite3_close(db);
            return true;
        }
        return false;
    }
    bool OrdersDAO::update(User& user, Order& order){
        if (!User::check_access(user))
            return false;

        if (OrdersDAO::check_weight(&order, CarsDAO::find_by_number(user, order.getCarNumber()))) {
            sqlite3 *db;

            int rc = sqlite3_open("../autopark.db", &db);
            if (rc != SQLITE_OK)
                return false;
            std::string sql = "UPDATE orders SET datetime = ?, driver_id = ?, car_number = ?, length = ?, cargo_weight = ?, cost = ? WHERE id = ?";
            sqlite3_stmt *stmt;
            if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int64(stmt, 1, order.getDatetime());
                sqlite3_bind_int64(stmt, 2, order.getDriverId());
                sqlite3_bind_text(stmt, 3, order.getCarNumber().c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_double(stmt, 4, order.getLength());
                sqlite3_bind_double(stmt, 5, order.getCargoWeight());
                sqlite3_bind_double(stmt, 6, order.getCost());
                sqlite3_bind_int64(stmt, 7, order.getId());
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            } else {
                return false;
            }

            sqlite3_close(db);
            return true;
        }
        return false;
    }
}