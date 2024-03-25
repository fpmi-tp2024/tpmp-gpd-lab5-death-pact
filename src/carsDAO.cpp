#include "sql/sqlite3.h"
#include "car_park/carsDAO.hpp"
#include "car_park/car.hpp"
#include "car_park/user.hpp"
#include "car_park/driver.hpp"
#include <string>

static int f_select(void *sql_data, int argc, char **argv, char **azColName){
    if (argc == 0) return 0;

    std::string car_data = std::string(argv[0] ? argv[0] : "NULL");
    for (int i = 1; i < argc; i++) {
        car_data += "," + std::string(argv[i] ? argv[i] : "NULL");
    }
    sql_data = (void*)car_data.c_str();
    return 0;
}

void CarsDAO::insert(Car& car){
    sqlite3 *db;
    char *zErrMsg = nullptr;
    char *sql_data;
    const char *sql;
    int rc;
    rc = sqlite3_open("../ext/autopark.db", &db);
    std::string sql_str = "INSERT INTO cars (number,brand,initial_mileage,capacity) "  \
                            "VALUES ('" + car.getNumber() + "', '" + car.getBrand() + "', " + std::to_string(car.getInitialMileage()) + ", " + std::to_string(car.getCapacity()) + "); ";
    sql = sql_str.c_str();
    rc = sqlite3_exec(db, sql, nullptr, (void*)sql_data, &zErrMsg);
    sqlite3_close(db);
}

Car* CarsDAO::find_by_number(User& user, std::string car_number){
    sqlite3 *db;
    char *zErrMsg = nullptr;
    char *sql_data = "";
    const char *sql;
    int rc;
    rc = sqlite3_open("../ext/autopark.db", &db);
    std::string sql_str = "SELECT * FROM cars WHERE number = '" + car_number + "';";
    sql = sql_str.c_str();
    rc = sqlite3_exec(db, sql, f_select, (void*)sql_data, &zErrMsg);
    sqlite3_close(db);
    return new Car(std::string(sql_data));
}

Car* CarsDAO::find_with_max_total_mileage(){
    sqlite3 *db;
    char *zErrMsg = 0;
    char *sql_data = "";
    const char *sql;
    int rc;
    rc = sqlite3_open("../ext/autopark.db", &db);                                                              // to check fucking sql requests
    std::string sql_str = "SELECT * FROM cars " \
                          "WHERE number = (SELECT car_number FROM orders GROUP BY car_number ORDER BY SUM(length) DESC LIMIT 1);";
    sql = sql_str.c_str();
    rc = sqlite3_exec(db, sql, f_select, (void*)sql_data, &zErrMsg);
    sqlite3_close(db);
    return new Car(std::string(sql_data));
}