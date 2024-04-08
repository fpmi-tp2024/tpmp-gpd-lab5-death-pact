#include "car_park/car_park.hpp"
#include "catch2/catch.hpp"
#include <vector>

using namespace car_park;

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

TEST_CASE("Test UsersDAO find","[UsersDAO]") {
    SECTION("Check existing user search") {
        User *user = UsersDAO::find("vasily324","1111");
        REQUIRE(user != nullptr);
        REQUIRE(user->get_type() == "admin");
        delete user;
    }
    SECTION("Check find with wrong password") {
        User* user = UsersDAO::find("vasily324","9999");
        REQUIRE(user == nullptr);
        delete user;
    }
    SECTION("Check non existing user search"){
        User* user = UsersDAO::find("random_login","random_pass");
        REQUIRE(user == nullptr);
        delete user;
    }
}

TEST_CASE("Test UsersDAO insert","[UsersDAO]") {
    SECTION("Check new user insertion")
    {
        User user("new_user1,client");
        REQUIRE(UsersDAO::insert(user,"0000"));
    }
    SECTION("Check existing user insertion"){
        User *user = UsersDAO::find("vasily324","1111");
        REQUIRE(!UsersDAO::insert(*user,"0000"));
        delete user;
    }
}

TEST_CASE("Test UsersDAO update","[UsersDAO]"){
    SECTION("Check update existing user"){
        User *user = UsersDAO::find("vasily324","1111");
        REQUIRE(UsersDAO::update(*user,"9999"));
        delete user;
    }
    SECTION("Check update non existing user"){
        User user("new_login,admin");
        REQUIRE(UsersDAO::update(user,"9999"));
        REQUIRE(UsersDAO::find(user.get_login(),"9999") == nullptr);
    }
    SECTION("Update password on its' own value"){
        User *user = UsersDAO::find("vasily324","9999");
        REQUIRE(UsersDAO::update(*user,"9999"));
        delete user;
    }
}
