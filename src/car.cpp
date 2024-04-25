#include "car_park/car.hpp"
#include "sql/sqlite3.h"
#include <string>
#include <sstream>

namespace car_park {
    Car::Car() {
        number = "";
        brand = "Unknown";
        initial_mileage = 0;
        capacity = 0;
        total_mileage = 0;
        total_weight = 0;
    }

    Car::Car(std::string sql_data) {
        std::stringstream ss(sql_data);
        std::getline(ss, number, ',');
        std::getline(ss, brand, ',');
        ss >> initial_mileage;
        ss.ignore();
        ss >> capacity;

        total_mileage = 0;
        total_weight = 0;
        count_totals();
    }

    void Car::count_totals() {
        sqlite3 *db;
        int rc = sqlite3_open("../../autopark.db", &db);
        if (rc != SQLITE_OK)
            return;
        std::string sql = "SELECT SUM(length) AS total_mileage, SUM(cargo_weight) AS total_cargo_weight FROM orders WHERE car_number = ?;";
        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, number.c_str(), -1, SQLITE_TRANSIENT);

            if (sqlite3_step(stmt) == SQLITE_ROW) {
                total_mileage = sqlite3_column_double(stmt, 0) + initial_mileage;
                total_weight = sqlite3_column_double(stmt, 1);

                sqlite3_finalize(stmt);
                sqlite3_close(db);
            }
        }
        sqlite3_close(db);
    }

    bool CarsDAO::insert(Car& car){
        sqlite3 *db;
        int rc = sqlite3_open("../../autopark.db", &db);
        if (rc != SQLITE_OK)
            return false;
        std::string sql = "INSERT INTO cars (number, brand, initial_mileage, capacity) VALUES (?, ?, ?, ?)";
        sqlite3_stmt* stmt;
        if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, car.getNumber().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, car.getBrand().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_double(stmt, 3, car.getInitialMileage());
            sqlite3_bind_double(stmt, 4, car.getCapacity());
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                return false;
            };
            sqlite3_finalize(stmt);
        }
        sqlite3_close(db);
        return true;
    }

    Car* CarsDAO::find_by_number(User& user, std::string car_number){
        if (Driver::check_ownership(user,car_number)) {
            sqlite3 *db;
            int rc = sqlite3_open("../../autopark.db", &db);
            if (rc != SQLITE_OK)
                return nullptr;
            std::string sql = "SELECT number,brand,initial_mileage,capacity FROM cars WHERE number = ?";
            sqlite3_stmt *stmt;

            if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_text(stmt, 1, car_number.c_str(), -1, SQLITE_TRANSIENT);

                if (sqlite3_step(stmt) == SQLITE_ROW) {
                    std::string number = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
                    std::string brand = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
                    double initial_mileage = sqlite3_column_double(stmt, 2);
                    double capacity = sqlite3_column_double(stmt, 3);

                    sqlite3_finalize(stmt);
                    sqlite3_close(db);
                    return new Car(number + "," + brand + "," + std::to_string(initial_mileage) + "," +
                                   std::to_string(capacity));
                }
            }
            sqlite3_close(db);
        }
        return nullptr;
    }

    Car* CarsDAO::find_with_max_total_mileage(User& user){
        sqlite3 *db;
        int rc = sqlite3_open("../../autopark.db", &db);
        if (rc != SQLITE_OK)
            return nullptr;
        std::string sql = "SELECT cars.number,cars.brand,cars.initial_mileage,cars.capacity, "
                          " (cars.initial_mileage + IFNULL(SUM(orders.length), 0)) AS total_mileage "
                          "FROM cars "
                          "LEFT JOIN orders ON cars.number = orders.car_number "
                          "GROUP BY cars.number "
                          "ORDER BY total_mileage DESC "
                          "LIMIT 1";

        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                std::string number = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
                std::string brand = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
                double initial_mileage = sqlite3_column_double(stmt, 2);
                double capacity = sqlite3_column_double(stmt, 3);
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                if (Driver::check_ownership(user,number)) {
                    return new Car(number + "," + brand + "," + std::to_string(initial_mileage) + "," +
                                   std::to_string(capacity));
                }
                return nullptr;
            }
        }
        sqlite3_close(db);
        return nullptr;
    }

    void CarsDAO::find_all(User& user, std::vector<Car>& cars){
        if (!User::check_access(user))
            return;

        sqlite3 *db;
        int rc = sqlite3_open("../../autopark.db", &db);
        if (rc != SQLITE_OK)
            return;
        std::string sql = "SELECT number,brand,initial_mileage,capacity FROM cars ORDER BY capacity ASC";

        sqlite3_stmt* stmt;

        if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                std::string number = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
                std::string brand = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
                double initial_mileage = sqlite3_column_double(stmt, 2);
                double capacity = sqlite3_column_double(stmt, 3);
                cars.emplace_back(Car(number + "," + brand + "," + std::to_string(initial_mileage) + "," +
                                      std::to_string(capacity)));
            }

            sqlite3_finalize(stmt);
        }
        sqlite3_close(db);
    }
}

