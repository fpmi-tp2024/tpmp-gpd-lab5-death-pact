#include "car_park/moneyPerPeriod.hpp"
#include "car_park/user.hpp"
#include "car_park/moneyPerPeriodDAO.hpp"
#include "car_park/driversDAO.hpp"
#include <vector>
#include "sql/sqlite3.h"
#include "car_park/driver.hpp"
static int f_select(void *sql_data, int argc, char **argv, char **azColName){
    if (argc == 0) return 0;

    std::string select_data = std::string(argv[0] ? argv[0] : "0.0");
    char* endptr;
    long double total_money = strtold(select_data.c_str(), &endptr);
    sql_data = (void*)(&total_money);
    return 0;
}

MoneyPerPeriod* MoneyPerPeriodDAO::count(User& user, Driver& driver, long long datetime_start, long long datetime_end){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    std::string query;
    int rc = sqlite3_open("../ext/autopark.db", &db);
    char *zErrMsg = nullptr;
    long double total_money = 0.0;
    if (rc == SQLITE_OK) {
        query = "SELECT SUM(cost) FROM orders WHERE driver_id = ?";                                    // BETWEEN start and end
        rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
        if (rc == SQLITE_OK) {
            sqlite3_bind_int64(stmt, 1, driver.getId());
            rc = sqlite3_step(stmt);
            if (rc == SQLITE_ROW) {
                total_money = sqlite3_column_double(stmt, 0);
            }
        }
    }
    std::string sql_data = std::to_string(datetime_start) + "," + std::to_string(datetime_end) + "," + std::to_string(driver.getId()) + "," + std::to_string(total_money);
    sqlite3_finalize(stmt);

    MoneyPerPeriod* new_note = new MoneyPerPeriod(sql_data);
    query = "INSERT INTO money_per_period (id,datetime_from,datetime_to,driver_id,total_money) "  \
                            "VALUES (" + std::to_string(new_note->getId()) + "," + sql_data + ");";
    rc = sqlite3_exec(db, query.c_str(), nullptr, (void*)sql_data.c_str(), &zErrMsg);
    delete[] zErrMsg;
    sqlite3_close(db);
    return new_note;
}

std::vector<MoneyPerPeriod*> MoneyPerPeriodDAO::count_all(User& user, long long datetime_start, long long datetime_end){
    std::vector<Driver> drivers(2);
    std::vector<MoneyPerPeriod*> money_notes;
    DriversDAO::find_all(user, drivers);
    for(auto i : drivers)
    {
        money_notes.push_back(count(user, i, datetime_start, datetime_end));
    }
    return money_notes;
}