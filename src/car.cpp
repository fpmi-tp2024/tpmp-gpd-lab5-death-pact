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
    }

    bool CarsDAO::insert(Car& car){
        sqlite3 *db;
        int rc = sqlite3_open("../../autopark.db", &db);
        if (rc != SQLITE_OK)
            return false;
        std::string sql = "INSERT INTO cars (number, brand, initial_mileage, capacity) VALUES (?, ?, ?, ?)";
        sqlite3_stmt* stmt;
        if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, car.getNumber().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, car.getBrand().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_double(stmt, 3, car.getInitialMileage());
            sqlite3_bind_double(stmt, 4, car.getCapacity());
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        } else {
            return false;
        }
        sqlite3_close(db);
        return true;
    }

    Car* CarsDAO::find_by_number(User& user, std::string car_number){
        if (User::check_access(user)) {
            sqlite3 *db;
            int rc = sqlite3_open("../../autopark.db", &db);
            if (rc != SQLITE_OK)
                return nullptr;
            std::string sql = "SELECT * FROM cars WHERE number = ?";
            sqlite3_stmt *stmt;

            if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_text(stmt, 1, car_number.c_str(), -1, SQLITE_STATIC);

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
        if (User::check_access(user)) {
            sqlite3 *db;
            int rc = sqlite3_open("../../autopark.db", &db);
            if (rc != SQLITE_OK)
                return nullptr;
            std::string sql = "SELECT cars.*, "
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
                    return new Car(number + "," + brand + "," + std::to_string(initial_mileage) + "," +
                                   std::to_string(capacity));
                }
            }
            sqlite3_close(db);
        }
        return nullptr;
    }
}