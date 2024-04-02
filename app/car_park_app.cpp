#include "car_park/user.hpp"
#include <iostream>
#include <string>

using namespace car_park;


/* TODO: 4. Создать функцию, который при добавлении информации в таблицу заказов,
проверяет, не превышает ли масса груза грузоподъемности машины, и если это так,
то запрещает помещать информацию в таблицу(*).
Вопрос по этому заданию: кто вносит заказ? Админ, водитель или тут всё-таки есть какой-то клиент?
*/

void showMainMenu() {
    std::cout << "Welcome to the car fleet management system!" << std::endl;
    std::cout << "Menu:" << std::endl;
    std::cout << "1. Log in" << std::endl;
    std::cout << "2. Exit the application" << std::endl;
}

void showDriverMenu() {
    std::cout << "Driver Menu:" << std::endl;
    std::cout << "1. View completed orders during a specific period" << std::endl;
    std::cout << "2. View total mileage and total cargo weight for a specific car" << std::endl;
    std::cout << "3. View total trips, total cargo weight, and earned money" << std::endl;
    std::cout << "4. View details for your car" << std::endl;
    std::cout << "5. Log out" << std::endl;
}

//TODO: create a driver object
//TODO: fill the cases with functions' executions
void driverMenu() {
    int choice;

    do {
        showDriverMenu();
        std::cout << "Choose an option (1-3): ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Viewing completed orders during a specific period" << std::endl;
                break;
            case 2:
                std::cout << "Viewing total mileage and total cargo weight for a specific car" << std::endl;
                break;
            case 3:
                std::cout << "Viewing total trips, total cargo weight, and earned money" << std::endl;
                break;
            case 4:
                std::cout << "Viewing details for your car" << std::endl;
                break;
            case 5:
                std::cout << "Logging out..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    } while (choice != 5);
}

void showFleetManagementMenu() {
    std::cout << "Fleet Management Menu:" << std::endl;
    std::cout << "1. View completed orders for a specific driver during a specified period" << std::endl;
    std::cout << "2. View total mileage and total cargo weight for a specific car" << std::endl;
    std::cout << "3. View total trips, total cargo weight, and earned money for each driver" << std::endl;
    std::cout << "4. View details and earnings for the driver with the fewest trips" << std::endl;
    std::cout << "5. View details for the car with the highest total mileage" << std::endl;
    std::cout << "6. Log out" << std::endl;
}

//TODO: create an admin object
//TODO: fill the cases with functions' executions
void fleetManagementMenu() {
    int choice;

    do {
        showFleetManagementMenu();
        std::cout << "Choose an option (1-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Viewing completed orders for a specific driver during a specified period" << std::endl;
                break;
            case 2:
                std::cout << "Viewing total mileage and total cargo weight for a specific car" << std::endl;
                break;
            case 3:
                std::cout << "Viewing total trips, total cargo weight, and earned money for each driver" << std::endl;
                break;
            case 4:
                std::cout << "Viewing details and earnings for the driver with the fewest trips" << std::endl;
                break;
            case 5:
                std::cout << "Viewing details for the car with the highest total mileage" << std::endl;
                break;
            case 6:
                std::cout << "Logging out..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    } while (choice != 6);
}

void login() {
    std::string username, password;

    std::cout << "Enter your username: ";
    std::cin >> username;

    std::cout << "Enter your password: ";
    std::cin >> password;

    UsersDAO usersDAO;

    User* user = usersDAO.find(username, password);

    if (user != nullptr) {
        std::string userRole = user->get_type();
        std::cout << "You have logged in as a " << userRole << "." << std::endl;

        if (userRole == "driver") {
            driverMenu();
        } else if (userRole == "admin") {
            fleetManagementMenu();
        } else {
            std::cout << "Invalid user role." << std::endl;
        }

        delete user;
    } else {
        std::cout << "Invalid username or password." << std::endl;
    }
}

int main() {
    int choice;

    do {
        showMainMenu();
        std::cout << "Choose an option (1-2): ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                login();
                break;
            case 2:
                std::cout << "Goodbye!" << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    } while (choice != 2);

    return 0;
}
