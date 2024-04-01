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

    }


    static int f_select(void *sql_data, int argc, char **argv, char **azColName){
        if (argc == 0) return 0;

        std::string driver_data = std::string(argv[0] ? argv[0] : "NULL");
        for (int i = 1; i < argc; i++) {
            driver_data += "," + std::string(argv[i] ? argv[i] : "NULL");
        }
        sql_data = (void*)driver_data.c_str();
        return 0;
    }

    static int f_select_all(void *driverListPtr, int argc, char **argv, char **azColName) {
        if (argc == 0) return 0;

        auto drivers = (std::vector<Driver>*)driverListPtr;
        std::string driver_data = std::string(argv[0] ? argv[0] : "NULL");
        for (int i = 1; i < argc; i++) {
            driver_data += "," + std::string(argv[i] ? argv[i] : "NULL");
        }
        drivers->emplace_back(Driver(driver_data));
        return 0;
    }

    Driver* DriversDAO::find_by_user(User& user){
        sqlite3 *db;
        char *zErrMsg = nullptr;
        char *sql_data;
        const char *sql;
        int rc;
        rc = sqlite3_open("../ext/autopark.db", &db);
        std::string sql_str = "SELECT * FROM drivers WHERE user_login = '" + user.get_login() + "'";
        sql = sql_str.c_str();
        rc = sqlite3_exec(db, sql, f_select, (void*)sql_data, &zErrMsg);
        sqlite3_close(db);
        return new Driver(std::string(sql_data));
    }
    Driver* DriversDAO::find_with_min_orders(User& user){
        sqlite3 *db;
        char *zErrMsg = nullptr;
        char *sql_data;
        const char *sql;
        int rc;
        rc = sqlite3_open("../ext/autopark.db", &db);
        std::string sql_str = "SELECT * FROM drivers " \
                          "INNER JOIN orders ON drivers.id = orders.driver_id " \
                          "GROUP BY id " \
                          "ORDER BY COUNT(id) ASC " \
                          "LIMIT 1; ";
        sql = sql_str.c_str();
        rc = sqlite3_exec(db, sql, f_select, (void*)sql_data, &zErrMsg);
        sqlite3_close(db);
        return new Driver(std::string(sql_data));
    }
    void DriversDAO::find_all(User& user, std::vector<Driver>& drivers){
        sqlite3 *db;
        char *zErrMsg = nullptr;
        std::vector<Driver>* driverListPtr = &drivers;
        const char *sql;
        int rc;
        rc = sqlite3_open("../ext/autopark.db", &db);
        std::string sql_str = "SELECT * FROM drivers;";
        sql = sql_str.c_str();
        rc = sqlite3_exec(db, sql, f_select_all, (void*)driverListPtr, &zErrMsg);
        sqlite3_close(db);
    }
    void DriversDAO::insert(Driver& driver){
        sqlite3 *db;
        char *zErrMsg = nullptr;
        char *sql_data;
        const char *sql;
        int rc;
        rc = sqlite3_open("../ext/autopark.db", &db);
        std::string sql_str = "INSERT INTO drivers (id,user_login,name,category,career_start,birth_year) "  \
                            "VALUES (" + std::to_string(driver.getId()) + ", '" + driver.getUserLogin() + "', '" + driver.getName() + "', '"
                              + driver.getCategory() + "', " + std::to_string(driver.getCareerStart()) + ", " + std::to_string(driver.getBirthYear()) + "); ";
        sql = sql_str.c_str();
        rc = sqlite3_exec(db, sql, nullptr, (void*)sql_data, &zErrMsg);
        sqlite3_close(db);
    }

}