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


TEST_CASE("Test CarsDAO find_by_number","[CarsDAO]"){
    User admin("super,admin");
    User driver("oleg324,driver");
    User client("new_client,client");
    SECTION("Check existing car search"){
        Car* car = CarsDAO::find_by_number(admin,"3089 AB-7");
        REQUIRE(car != nullptr);
        REQUIRE(car->getBrand() == "BMW");
        delete car;
    }
    SECTION("Check non existing car search"){
        Car* car = CarsDAO::find_by_number(admin,"1456 IK-2");
        REQUIRE(car == nullptr);
        delete car;
    }
    SECTION("Check car search without permission"){
        Car* car = CarsDAO::find_by_number(client,"3089 AB-7");
        REQUIRE(car == nullptr);
        delete car;
    }
    SECTION("Check car search by wrong driver"){
        Car* car = CarsDAO::find_by_number(driver,"3089 AB-7");
        REQUIRE(car == nullptr);
        delete car;
    }
    SECTION("Check car search by owner"){
        Car* car = CarsDAO::find_by_number(driver,"8924 HP-3");
        REQUIRE(car->getBrand() == "Audi");
        delete car;
    }
}

TEST_CASE("Test CarsDAO find_with_max_total_mileage","[CarsDAO]"){
    User admin("super,admin");
    User owner("oleg324,driver");
    User not_owner("ivan228,driver");
    User client("new_client,client");
    SECTION("Check car search with admin rights"){
        Car* car = CarsDAO::find_with_max_total_mileage(admin);
        REQUIRE(car != nullptr);
        REQUIRE(car->getBrand() == "Audi");
        delete car;
    }
    SECTION("Check car search without permission"){
        Car* car = CarsDAO::find_with_max_total_mileage(client);
        REQUIRE(car == nullptr);
        delete car;
    }
    SECTION("Check car search by wrong driver"){
        Car* car = CarsDAO::find_with_max_total_mileage(not_owner);
        REQUIRE(car == nullptr);
        delete car;
    }
    SECTION("Check car search by owner"){
        Car* car = CarsDAO::find_with_max_total_mileage(owner);
        REQUIRE(car->getBrand() == "Audi");
        delete car;
    }
}

TEST_CASE("Test CarsDAO insert","[CarsDAO]") {
    User admin("super,admin");
    SECTION("Check new car insertion")
    {
        Car car("1456 IK-2,Fiat,120000,1000");
        REQUIRE(CarsDAO::insert(car));
    }
    SECTION("Check existing car insertion"){
        Car *car = CarsDAO::find_by_number(admin,"1456 IK-2");
        REQUIRE(!CarsDAO::insert(*car));
        delete car;
    }
}

TEST_CASE("Test MoneyPerPeriodDAO count","[MoneyPerPeriodDAO]"){
    User* admin = UsersDAO::find("vasily324","1111");
    User* driver = UsersDAO::find("oleg324","187536");
    Driver* active_driver = DriversDAO::find_by_user(*driver);
    Driver* inactive_driver = DriversDAO::find_by_user(*admin);
    SECTION("Check money count of active driver"){
        MoneyPerPeriod* mpp = MoneyPerPeriodDAO::count(*admin,*active_driver,20240303,20240308);
        REQUIRE(mpp->getTotalMoney() == 2600);
        delete mpp;
    }
    SECTION("Check money count on period with no orders"){
        MoneyPerPeriod* mpp = MoneyPerPeriodDAO::count(*admin,*active_driver,20240309,20240310);
        REQUIRE(mpp->getTotalMoney() == 0);
        delete mpp;
    }
    SECTION("Check money count of inactive driver"){                    // Driver with no completed orders
        MoneyPerPeriod* mpp = MoneyPerPeriodDAO::count(*admin,*inactive_driver,20240303,20240308);
        REQUIRE(mpp->getTotalMoney() == 0);
        delete mpp;
    }
    SECTION("Check money count without permission"){
        MoneyPerPeriod* mpp = MoneyPerPeriodDAO::count(*driver,*active_driver,20240303,20240308);
        REQUIRE(mpp == nullptr);
    }
    delete admin;
    delete driver;
    delete active_driver;
    delete inactive_driver;
}

TEST_CASE("Test MoneyPerPeriodDAO count_all","[MoneyPerPeriodDAO]"){
    User* admin = UsersDAO::find("vasily324","1111");
    User* client = UsersDAO::find("oleg324","187536");
    SECTION("Check count all with authorized user"){
        std::vector<MoneyPerPeriod*> mpps = MoneyPerPeriodDAO::count_all(*admin,20240303,20240308);
        REQUIRE(mpps.size() == 2);
        REQUIRE(mpps[0]->getTotalMoney() == 0);
        REQUIRE(mpps[1]->getTotalMoney() == 2600);
        for (auto i : mpps)
            delete i;
    }
    SECTION("Check count all without permission"){
        std::vector<MoneyPerPeriod*> mpps = MoneyPerPeriodDAO::count_all(*client,20240303,20240308);
        REQUIRE(mpps.empty());
    }
}
