#include "car_park/user.hpp"
#include "sql/sqlite3.h"
#include <sstream>




namespace car_park {
    User::User() {
        login = "";
        type = "client";
    }

    User::User(std::string sql_data) {
        std::stringstream ss(sql_data);
        std::getline(ss, login, ',');
        std::getline(ss, type, ',');
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
        int rc = sqlite3_open("../../autopark.db", &db);
        if (rc != SQLITE_OK)
            return nullptr;
        std::string sql = "SELECT * FROM users WHERE login = ? AND password = ? ";
        sqlite3_stmt* stmt;

        if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                std::string user_login = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                std::string type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));


                sqlite3_finalize(stmt);
                sqlite3_close(db);
                return new User(user_login + "," + type);
            }
        }
        sqlite3_close(db);
        return nullptr;
    }

    bool UsersDAO::insert(User &user, std::string password) {
        sqlite3 *db;
        int rc = sqlite3_open("../../autopark.db", &db);
        if (rc != SQLITE_OK)
            return false;
        std::string sql = "INSERT INTO users (login, password, role) VALUES (?, ?, ?)";
        sqlite3_stmt* stmt;
        if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, user.get_login().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, user.get_type().c_str(), -1, SQLITE_STATIC);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        } else {
            return false;
        }

        sqlite3_close(db);
        return true;
    }

    bool UsersDAO::update(User &user, std::string password) {
        sqlite3 *db;
        int rc = sqlite3_open("../../autopark.db", &db);
        if (rc != SQLITE_OK)
            return false;
        std::string sql = "UPDATE users SET password = ?, role = ? WHERE login = ?";
        sqlite3_stmt* stmt;
        if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, password.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, user.get_type().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, user.get_login().c_str(), -1, SQLITE_STATIC);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        } else {
            return false;
        }

        sqlite3_close(db);
        return true;
    }
}

