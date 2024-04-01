#include "car_park/user.hpp"

#include "sql/sqlite3.h"
#include <cstring>
#include <sstream>




namespace car_park {
    User::User() {
        login = "";
        type = "superuser";
    }

    User::User(std::string sql_data) {
        const char *delim = ",";
        char *value = strtok(sql_data.data(), delim);
        login = std::string(value);
        value = strtok(nullptr, delim);
        type = std::string(value);
    }

    std::string User::get_login() {
        return login;
    }

    std::string User::get_type() {
        return type;
    }


    static int f_select(void *sql_data, int argc, char **argv, char **azColName) {
        if (argc == 0) return 0;

        std::string user_data = std::string(argv[0] ? argv[0] : "NULL") + "," + std::string(argv[1] ? argv[1] : "NULL");
        sql_data = (void *) user_data.c_str();
        return 0;
    }

    User *UsersDAO::find(std::string login, std::string password) {
        sqlite3 *db;
        char *zErrMsg = nullptr;
        char *sql_data;
        const char *sql;
        int rc;
        rc = sqlite3_open("../ext/autopark.db", &db);
        std::string sql_str = "SELECT * FROM users WHERE login = '" + login + "' AND password = '" + password + "';";
        sql = sql_str.c_str();
        rc = sqlite3_exec(db, sql, f_select, (void *) sql_data, &zErrMsg);
        sqlite3_close(db);
        return new User(std::string(sql_data));
    }

    void UsersDAO::insert(User &user, std::string password) {
        sqlite3 *db;
        char *zErrMsg = nullptr;
        char *sql_data;
        const char *sql;
        int rc;
        rc = sqlite3_open("../ext/autopark.db", &db);
        std::string sql_str = "INSERT INTO users (login,password,role) "  \
                            "VALUES ('" + user.get_login() + "', '" + password + "', '" + user.get_type() + "'); ";
        sql = sql_str.c_str();
        rc = sqlite3_exec(db, sql, nullptr, (void *) sql_data, &zErrMsg);
        sqlite3_close(db);
    }

    void UsersDAO::update(User &user, std::string password) {
        sqlite3 *db;
        char *zErrMsg = nullptr;
        char *sql_data;
        const char *sql;
        int rc;
        rc = sqlite3_open("../ext/autopark.db", &db);
        std::string sql_str =
                "UPDATE users SET password = '" + password + "' WHERE login = '" + user.get_login() + "';";
        sql = sql_str.c_str();
        rc = sqlite3_exec(db, sql, nullptr, (void *) sql_data, &zErrMsg);
        sqlite3_close(db);
    }
}

