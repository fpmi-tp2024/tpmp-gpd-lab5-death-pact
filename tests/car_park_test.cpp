#include "car_park/car_park.hpp"
#include "catch2/catch.hpp"
#include <vector>
#include "sql/sqlite3.h"
#include <fstream>
#include <filesystem>
#include <iostream>

using namespace car_park;

bool isCreated = false;

bool createDB(){
    if (isCreated)
        return false;
    std::ifstream file("../static/db_migration/createDB.sql");
    std::string line;
    std::string sql = " ";
    while (std::getline(file, line)){
        sql += line;
    }
    std::cout << "debug log !!! " << std::filesystem::current_path() << std::endl;
    std::string path = "../tests";
    for (const auto & entry : std::filesystem::directory_iterator(path))
        std::cout << entry.path() << std::endl;
    std::cout << "-------\n";
    path = "..";
    for (const auto & entry : std::filesystem::directory_iterator(path))
        std::cout << entry.path() << std::endl;
    if(sql == " ")
        throw std::invalid_argument("oh shit");
    sqlite3 *db;
    std::filesystem::remove("../../autopark.db");
    int rc = sqlite3_open("../../autopark.db", &db);
    char *errMsg = 0;
    rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK){
        sqlite3_free(errMsg);
        sqlite3_close(db);
        file.close();
        isCreated = true;
        return false;
    }
    sqlite3_free(errMsg);
    sqlite3_close(db);
    file.close();
    isCreated = true;
    return true;
}


TEST_CASE("Test User constructor", "[User]") {
    User user("user_login,driver");
    SECTION("Check User object, created from sql_data"){
        REQUIRE(user.get_login() == "user_login");
        REQUIRE(user.get_type() == "driver");
    }
    User user_d;
    SECTION("Check User default constructor"){
        REQUIRE(user_d.get_type() == "client");
        REQUIRE(user_d.get_login().empty());
    }
}

TEST_CASE("Test UsersDAO insert","[UsersDAO]") {
    SECTION("Check new user insertion")
    {
        createDB();
        User user("new_user1,client");
        REQUIRE(UsersDAO::insert(user,"0000"));
    }
    SECTION("Check existing user insertion"){
        createDB();
        User u("new_user1,client");
        UsersDAO::insert(u,"0000");
        User *user = UsersDAO::find("new_user1","0000");
        REQUIRE(!UsersDAO::insert(*user,"9999"));
        delete user;
    }
}

TEST_CASE("Test UsersDAO find","[UsersDAO]") {
    SECTION("Check existing user search") {
        createDB();
        User *user_find = UsersDAO::find("oleg324","187536");
        REQUIRE(user_find != nullptr);
        REQUIRE(user_find->get_type() == "driver");
        delete user_find;
    }
    SECTION("Check find with wrong password") {
        createDB();
        User* user = UsersDAO::find("oleg324","9999");
        REQUIRE(user == nullptr);
        delete user;
    }
    SECTION("Check non existing user search"){
        createDB();
        User* user = UsersDAO::find("random_login","random_pass");
        REQUIRE(user == nullptr);
        delete user;
    }
}

TEST_CASE("Test UsersDAO update","[UsersDAO]"){
    SECTION("Check update existing user"){
        createDB();
        User user("new_user2,client");
        UsersDAO::insert(user,"0000");
        REQUIRE(UsersDAO::update(user,"9999"));
    }
    SECTION("Check update non existing user"){
        createDB();
        User user("new_login,admin");
        REQUIRE(UsersDAO::update(user,"9999"));
        REQUIRE(UsersDAO::find(user.get_login(),"9999") == nullptr);
    }
    SECTION("Update password on its' own value"){
        createDB();
        User *user = UsersDAO::find("new_user2","9999");
        REQUIRE(UsersDAO::update(*user,"9999"));
        delete user;
    }
}

