#include "car_park/driver.hpp"
#include "sql/sqlite3.h"
#include <sstream>

namespace car_park {
    Driver::Driver(std::string sql_data) {
        std::stringstream ss(sql_data);
        ss >> id;
        ss.ignore();
        std::getline(ss, user_login, ',');
        std::getline(ss, name, ',');
        std::getline(ss, category, ',');
        ss >> career_start;
        ss.ignore();
        ss >> birth_year;

        total_weight = 0;
        total_money = 0;
        total_orders = 0;
    }

    int Driver::count_order_per_period(long long start_date, long long end_date) {
        sqlite3 *db;
        int rc = sqlite3_open("../../autopark.db", &db);
        if (rc != SQLITE_OK)
            return 0;
        std::string sql = "SELECT COUNT(*) FROM orders WHERE driver_id = ? AND datetime >= ? AND datetime <= ?";
        sqlite3_stmt* stmt;
        int count = 0;

        if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int64(stmt, 1, id);
            sqlite3_bind_int64(stmt, 2, start_date);
            sqlite3_bind_int64(stmt, 3, end_date);

            if (sqlite3_step(stmt) == SQLITE_ROW) {
                count = sqlite3_column_int(stmt, 0);
            }

            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return count;
        }
        sqlite3_close(db);
        return 0;
    }



    Driver* DriversDAO::find_by_user(User& user){
        sqlite3 *db;
        int rc = sqlite3_open("../../autopark.db", &db);
        if (rc != SQLITE_OK)
            return nullptr;
        std::string sql = "SELECT * FROM drivers WHERE user_login = ?";
        sqlite3_stmt* stmt;

        if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, user.get_login().c_str(), -1, SQLITE_STATIC);

            if (sqlite3_step(stmt) == SQLITE_ROW) {
                int id = sqlite3_column_int(stmt, 0);
                std::string user_login = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                std::string category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                int career_start = sqlite3_column_int(stmt, 4);
                int birth_year = sqlite3_column_int(stmt, 5);

                sqlite3_finalize(stmt);
                sqlite3_close(db);
                return new Driver(std::to_string(id) + "," + user_login + "," + name + "," + category + "," +
                                  std::to_string(career_start) + "," + std::to_string(birth_year));
            }
        }
        sqlite3_close(db);
        return nullptr;
    }
    Driver* DriversDAO::find_with_min_orders(){
        sqlite3 *db;
        int rc = sqlite3_open("../../autopark.db", &db);
        if (rc != SQLITE_OK)
            return nullptr;
        std::string sql = "SELECT drivers.*, COUNT(orders.id) AS total_orders "
                          "FROM drivers "
                          "LEFT JOIN orders ON drivers.id = orders.driver_id "
                          "GROUP BY drivers.id "
                          "ORDER BY total_orders ASC "
                          "LIMIT 1";

        sqlite3_stmt* stmt;

        if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                int id = sqlite3_column_int(stmt, 0);
                std::string user_login = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                std::string category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                int career_start = sqlite3_column_int(stmt, 4);
                int birth_year = sqlite3_column_int(stmt, 5);



                sqlite3_finalize(stmt);
                sqlite3_close(db);
                return new Driver(std::to_string(id) + "," + user_login + "," + name + "," + category + "," + std::to_string(career_start) + "," + std::to_string(birth_year));
            }
        }
        sqlite3_close(db);
        return nullptr;
    }
    void DriversDAO::find_all(User& user, std::vector<Driver>& drivers){
        sqlite3 *db;
        int rc = sqlite3_open("../../autopark.db", &db);
        if (rc != SQLITE_OK)
            return;
        std::string sql = "SELECT * FROM drivers";

        sqlite3_stmt* stmt;

        if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                int id = sqlite3_column_int(stmt, 0);
                std::string user_login = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                std::string category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                int career_start = sqlite3_column_int(stmt, 4);
                int birth_year = sqlite3_column_int(stmt, 5);

                drivers.emplace_back(Driver(std::to_string(id) + "," + user_login + "," + name + "," + category + "," + std::to_string(career_start) + "," + std::to_string(birth_year)));
            }

            sqlite3_finalize(stmt);
        }
        sqlite3_close(db);
    }
    bool DriversDAO::insert(Driver& driver){
        sqlite3 *db;
        int rc = sqlite3_open("../../autopark.db", &db);
        if (rc != SQLITE_OK)
            return false;
        std::string sql = "INSERT INTO drivers (user_login, name, category, career_start, birth_year) VALUES (?, ?, ?, ?, ?)";
        sqlite3_stmt* stmt;
        if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, driver.getUserLogin().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, driver.getName().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, driver.getCategory().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int64(stmt, 4, driver.getCareerStart());
            sqlite3_bind_int64(stmt, 5, driver.getBirthYear());
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        } else {
            sqlite3_close(db);
            return false;
        }
        sqlite3_close(db);
        return true;
    }

}