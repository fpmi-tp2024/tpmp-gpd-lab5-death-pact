#ifndef CAR_PARK_PROJECT_USER_H
#define CAR_PARK_PROJECT_USER_H

#pragma once
#include <string>

namespace car_park {
    class User {
    private:
        std::string login;
        std::string type;
    public:
        User();

        explicit User(std::string sql_data);

        std::string get_login();

        std::string get_type();
    };
    
    class UsersDAO {
    public:
        static User* find(std::string login, std::string password);
        static bool insert(User& user, std::string password);
        static bool update(User& user, std::string password);
    };

}

#endif //CAR_PARK_PROJECT_USER_H
