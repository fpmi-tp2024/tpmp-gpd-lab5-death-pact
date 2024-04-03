#include "car_park/car_park.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <ctime>

using namespace car_park;


/* TODO: 4. Создать функцию, который при добавлении информации в таблицу заказов,
проверяет, не превышает ли масса груза грузоподъемности машины, и если это так,
то запрещает помещать информацию в таблицу(*).
*/

long long dateToTimestamp(const std::string& dateString) {
    std::tm time = {};
    std::istringstream ss(dateString);
    ss >> std::get_time(&time, "%Y-%m-%d");

    if (ss.fail()) {
        std::cerr << "Parsing data mistake" << std::endl;
        return -1;
    }

    std::time_t timestamp = std::mktime(&time);
    return static_cast<long long>(timestamp);
}

void showMainMenu() {
    std::cout << "Welcome to the car fleet management system!" << std::endl;
    std::cout << "Menu:" << std::endl;
    std::cout << "1. Log in" << std::endl;
    std::cout << "2. Exit the application" << std::endl;
    std::cout << std::endl;
}

void showDriverMenu() {
    std::cout << "Driver Menu:" << std::endl;
    std::cout << "1. View completed orders during a specific period" << std::endl;
    std::cout << "2. View total mileage and total cargo weight for your car" << std::endl;
    std::cout << "3. View total trips, total cargo weight, and earned money" << std::endl;
    std::cout << "4. View details for your car" << std::endl;
    std::cout << "5. Log out" << std::endl;
    std::cout << std::endl;
}


void driverMenu(User* user) {
    int choice;
    Driver* driver = DriversDAO::find_by_user(*user);
    Car* car;
    std::cin.ignore(); // Ignore the remaining newline character
    do {
        showDriverMenu();
        std::cout << "Choose an option (1-5): ";
        std::string choiceStr;
        std::getline(std::cin, choiceStr);
        choice = std::stoi(choiceStr);
        std::string carNumber, inputDate1, inputDate2;
        long long timestamp1, timestamp2;
        switch (choice) {
            case 1:
                std::cout << "Viewing completed orders during a specific period\n" << std::endl;
                std::cout << "Enter first date yyyy-mm-dd: ";
                std::getline(std::cin, inputDate1);
                std::cout << "Enter second date yyyy-mm-dd: ";
                std::getline(std::cin, inputDate2);
                timestamp1 = dateToTimestamp(inputDate1);
                timestamp2 = dateToTimestamp(inputDate2);
                std::cout << "Completed " << driver->getName() << "'s orders per specific period: " << driver->count_order_per_period(timestamp1, timestamp2) << std::endl;
                std::cout << std::endl;
                break;
            case 2:
                std::cout << "Viewing total mileage and total cargo for your car\n" << std::endl;
                std::cout << "Enter your car number: ";
                std::getline(std::cin, carNumber);
                car = CarsDAO::find_by_number(*user, carNumber);
                if (car == nullptr) {
                    std::cout << "It's not your car number! \n Back to menu.";
                    break;
                }
                std::cout << "Total mileage: " << car->getTotalMileage() << std::endl;
                std::cout << "Total cargo weight: " << car->getTotalWeight() << std::endl;
                std::cout << std::endl;
                break;
            case 3:
                std::cout << "Viewing total trips, total cargo weight, and earned money\n" << std::endl;
                std::cout << "Total trips: " << driver->getTotalOrders() << std::endl;
                std::cout << "Total cargo weight: " << driver->getTotalWeight() << std::endl;
                std::cout << "Total earned money: " << driver->getTotalMoney() << std::endl;
                std::cout << std::endl;
                break;
            case 4:
                std::cout << "Viewing details for your car\n" << std::endl;
                std::cout << "Enter your car number: ";
                std::getline(std::cin, carNumber);
                car = CarsDAO::find_by_number(*user, carNumber);
                if (car == nullptr) {
                    std::cout << "It's not your car number! \n Back to menu.";
                    break;
                }
                std::cout << "Car number: " << car->getNumber() << std::endl;
                std::cout << "Brand: " << car->getBrand() << std::endl;
                std::cout << "Capacity: " << car->getCapacity() << std::endl;
                std::cout << "Initial mileage: " << car->getInitialMileage() << std::endl;
                std::cout << "Total mileage: " << car->getTotalMileage() << std::endl;
                std::cout << "Total cargo weight: " << car->getTotalWeight() << std::endl;
                std::cout << std::endl;
                break;
            case 5:
                std::cout << "Logging out...\n" << std::endl;
                std::cout << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                std::cout << std::endl;
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
    std::cout << std::endl;
}

void fleetManagementMenu(User* user) {
    int choice;
    Car* car;
    Driver* driver;
    User* user1;
    std::cin.ignore(); // Ignore the remaining newline character
    do {
        showFleetManagementMenu();
        std::cout << "Choose an option (1-6): ";
        std::string choiceStr;
        std::getline(std::cin, choiceStr);
        choice = std::stoi(choiceStr);
        std::string carNumber, userName, inputDate1, inputDate2;
        long long timestamp1, timestamp2;
        std::vector<Driver> drivers;

        switch (choice) {
            case 1:
                std::cout << "Viewing completed orders for a specific driver during a specified period \n" << std::endl;
                std::cout << "Enter driver's username: ";
                std::getline(std::cin, userName);
                user1 = new User(userName + ",driver");
                driver = DriversDAO::find_by_user(*user1);
                std::cout << "Enter first date yyyy-mm-dd: ";
                std::getline(std::cin, inputDate1);
                std::cout << "Enter second date yyyy-mm-dd: ";
                std::getline(std::cin, inputDate2);
                timestamp1 = dateToTimestamp(inputDate1);
                timestamp2 = dateToTimestamp(inputDate2);
                std::cout << "Completed " << driver->getName() << "'s orders per specific period: " << driver->count_order_per_period(timestamp1, timestamp2) << std::endl;
                std::cout << std::endl;
                delete user1;
                break;
            case 2:
                std::cout << "Viewing total mileage and total cargo weight for a specific car\n" << std::endl;
                std::cout << "Enter car number: ";
                std::getline(std::cin, carNumber);
                car = CarsDAO::find_by_number(*user, carNumber);
                std::cout << "Total mileage: " << car->getTotalMileage() << std::endl;
                std::cout << "Total cargo weight: " << car->getTotalWeight() << std::endl;
                std::cout << std::endl;
                break;
            case 3:
                std::cout << "Viewing total trips, total cargo weight, and earned money for each driver\n" << std::endl;
                DriversDAO::find_all(drivers);
                for (auto dr : drivers) {
                    std::cout << "Driver's name: " << dr.getName() << std::endl;
                    std::cout << "Total trips: " << dr.getTotalOrders() << std::endl;
                    std::cout << "Total cargo weight: " << dr.getTotalWeight() << std::endl;
                    std::cout << "Total earned money: " << dr.getTotalMoney() << std::endl;
                    std::cout << std::endl;
                }
                break;
            case 4:
                std::cout << "Viewing details and earnings for the driver with the fewest trips\n" << std::endl;
                driver = DriversDAO::find_with_min_orders();
                std::cout << "ID: " << driver->getId() << std::endl;
                std::cout << "User Login: " << driver->getUserLogin() << std::endl;
                std::cout << "Name: " << driver->getName() << std::endl;
                std::cout << "Category: " << driver->getCategory() << std::endl;
                std::cout << "Career Start: " << driver->getCareerStart() << std::endl;
                std::cout << "Birth Year: " << driver->getBirthYear() << std::endl;
                std::cout << "Total Orders: " << driver->getTotalOrders() << std::endl;
                std::cout << "Total Weight: " << driver->getTotalWeight() << std::endl;
                std::cout << "Total Money: " << driver->getTotalMoney() << std::endl;
                std::cout << std::endl;
                break;
            case 5:
                std::cout << "Viewing details for the car with the highest total mileage\n" << std::endl;
                car = CarsDAO::find_with_max_total_mileage(*user);
                std::cout << "Car number: " << car->getNumber() << std::endl;
                std::cout << "Brand: " << car->getBrand() << std::endl;
                std::cout << "Capacity: " << car->getCapacity() << std::endl;
                std::cout << "Initial mileage: " << car->getInitialMileage() << std::endl;
                std::cout << "Total mileage: " << car->getTotalMileage() << std::endl;
                std::cout << "Total cargo weight: " << car->getTotalWeight() << std::endl;
                std::cout << std::endl;
                break;
            case 6:
                std::cout << "Logging out..." << std::endl;
                std::cout << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                std::cout << std::endl;
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
            driverMenu(user);
        } else if (userRole == "admin") {
            fleetManagementMenu(user);
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
