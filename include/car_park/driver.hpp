#ifndef CAR_PARK_PROJECT_DRIVER_H
#define CAR_PARK_PROJECT_DRIVER_H
#include <string>
#include <vector>
#include "car_park/user.hpp"

namespace car_park {
    class Driver {
    private:
        Driver();
        long long id;
        std::string user_login;
        std::string name;
        std::string category;
        long long career_start;
        long long birth_year;
        int total_orders;
        long double total_weight;
        long double total_money;
    public:
        explicit Driver(std::string sql_data);

        long long getId() {
            return id;
        }

        std::string getUserLogin() {
            return user_login;
        }

        std::string getName() {
            return name;
        }

        std::string getCategory() {
            return category;
        }

        long long getCareerStart() {
            return career_start;
        }

        long long getBirthYear() {
            return birth_year;
        }

        int getTotal() {
            return total_orders;
        }

        long double getTotalWeight() {
            return total_weight;
        }

        long double getTotalMoney() {
            return total_money;
        }

        int count_order_per_period(long long start_date, long long end_date);
    };
    class DriversDAO {
    public:
        static Driver* find_by_user(User& user);
        static Driver* find_with_min_orders();
        static void find_all(User& user, std::vector<Driver>& drivers);
        static bool insert(Driver& driver);
    };
}
#endif //CAR_PARK_PROJECT_DRIVER_H
