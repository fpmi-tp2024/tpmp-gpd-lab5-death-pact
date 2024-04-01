#include "car_park/moneyPerPeriod.hpp"
#include "sql/sqlite3.h"
#include <sstream>
namespace car_park {
    long long MoneyPerPeriod::next_id = 0;
    MoneyPerPeriod::MoneyPerPeriod(std::string sql_data) : id(next_id++) {
        std::stringstream ss(sql_data);
        ss >> datetime_from;
        ss.ignore();
        ss >> datetime_to;
        ss.ignore();
        ss >> driver_id;
        ss.ignore();
        ss >> total_money;
    }


    MoneyPerPeriod* MoneyPerPeriodDAO::count(User& user, Driver& driver, long long datetime_start, long long datetime_end){
        if (User::check_access(user)) {
            double total_money = 0.0;
            sqlite3 *db;
            int rc = sqlite3_open("../../autopark.db", &db);
            if (rc != SQLITE_OK)
                return nullptr;
            std::string sql = "SELECT IFNULL(SUM(cost), 0) AS total_money FROM orders WHERE driver_id = ? AND datetime >= ? AND datetime <= ?";
            sqlite3_stmt *stmt;

            if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int64(stmt, 1, driver.getId());
                sqlite3_bind_int64(stmt, 2, datetime_start);
                sqlite3_bind_int64(stmt, 3, datetime_end);

                if (sqlite3_step(stmt) == SQLITE_ROW) {
                    total_money = sqlite3_column_double(stmt, 0);
                }

                sqlite3_finalize(stmt);
            } else {
                return nullptr;
            }

            MoneyPerPeriod *money_per_period = new MoneyPerPeriod(
                    std::to_string(datetime_start) + "," + std::to_string(datetime_end) + ","
                    + std::to_string(driver.getId()) + "," + std::to_string(total_money));

            sql = "INSERT INTO money_per_period (datetime_from, datetime_to, driver_id, total_money) VALUES (?, ?, ?, ?)";

            if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int64(stmt, 1, datetime_start);
                sqlite3_bind_int64(stmt, 2, datetime_end);
                sqlite3_bind_int64(stmt, 3, driver.getId());
                sqlite3_bind_double(stmt, 4, total_money);
                sqlite3_finalize(stmt);
            } else {
                return nullptr;
            }
            sqlite3_close(db);
            return money_per_period;
        }
        return nullptr;
    }


    std::vector<MoneyPerPeriod*> MoneyPerPeriodDAO::count_all(User& user, long long datetime_start, long long datetime_end){
        std::vector<MoneyPerPeriod*> money_notes;
        if (User::check_access(user)) {
            std::vector<Driver> drivers;
            DriversDAO::find_all(drivers);
            for (auto i: drivers) {
                money_notes.push_back(count(user, i, datetime_start, datetime_end));
            }
        }
        return money_notes;
    }

}