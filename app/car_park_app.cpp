#include "car_park/car_park.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <ctime>

using namespace car_park;


long long dateToTimestamp(const std::string& dateString) {
    std::stringstream ss(dateString);
    long long year;
    long long month;
    long long day;
    ss >> year;
    ss.ignore();
    ss >> month;
    ss.ignore();
    ss >> day;
    if (ss.fail()) {
        std::cerr << "Parsing data mistake" << std::endl;
        return -1;
    }
    return year * 10000L + month * 100L + day;
}

void showMainMenu() {
    std::cout << "Welcome to the car fleet management system!" << std::endl;
    std::cout << "Menu:" << std::endl;
    std::cout << "1. Log in" << std::endl;
    std::cout << "2. New order" << std::endl;
    std::cout << "3. Exit the application" << std::endl;
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
    std::vector<Order> orders;
    std::cin.ignore();
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
                OrdersDAO::count_order_per_period(*driver, orders, timestamp1,timestamp2);
                std::cout << "Completed " << driver->getName() << "'s orders per specific period: " << orders.size() << " order(s)" << std::endl;
                for (auto order : orders) {
                    std::cout << "Order ID: " << order.getId() << std::endl;
                    std::cout << "Driver's car number: " << order.getCarNumber() << std::endl;
                    std::cout << "Order fee: " << order.getCost() << std::endl << std::endl;
                }
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
                delete car;
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
    delete driver;
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
    std::cin.ignore();
    do {
        showFleetManagementMenu();
        std::cout << "Choose an option (1-6): ";
        std::string choiceStr;
        std::getline(std::cin, choiceStr);
        choice = std::stoi(choiceStr);
        std::string carNumber, userName, inputDate1, inputDate2;
        long long timestamp1, timestamp2;
        std::vector<Driver> drivers;
        std::vector<Order> orders;
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
                OrdersDAO::count_order_per_period(*driver, orders, timestamp1,timestamp2);
                std::cout << "Completed " << driver->getName() << "'s orders per specific period: " << orders.size() << " order(s)" << std::endl;
                for (auto order : orders) {
                    std::cout << "Order ID: " << order.getId() << std::endl;
                    std::cout << "Driver's car number: " << order.getCarNumber() << std::endl;
                    std::cout << "Order fee: " << order.getCost() << std::endl << std::endl;
                }
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
                delete car;
                break;
            case 3:
                std::cout << "Viewing total trips, total cargo weight, and earned money for each driver\n" << std::endl;
                DriversDAO::find_all(*user,drivers);
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
                driver = DriversDAO::find_with_min_orders(*user);
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
                delete driver;
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
                delete car;
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

    User* user = UsersDAO::find(username, password);

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

long long getCurrentDateAsLongLong() {
    time_t now = time(nullptr);
    if (now == -1) {
        // Error occurred while getting current time
        return -1;
    }

    tm *currentTime = localtime(&now);
    if (currentTime == nullptr) {
        // Error occurred while converting time
        return -1;
    }

    long long currentDate = (currentTime->tm_year + 1900) * 10000L + (currentTime->tm_mon + 1) * 100L + currentTime->tm_mday;
    return currentDate;
}

void newOrder() {
    std::string destination, weight, userName, carNumber;
    std::cin.ignore();
    User* user = new User(userName + ",admin");
    std::cout << "Enter car number: ";
    std::getline(std::cin, carNumber);
    Car *car = CarsDAO::find_by_number(*user, carNumber);
    if (car == nullptr) {
        std::cout << "There's no such car number in database! \n Back to the main menu... \n" << std::endl;
        return;
    }
    long long date = getCurrentDateAsLongLong();
    std::cout << date;
    std::cout << "Enter the destination (.0 km): ";
    std::getline(std::cin, destination);
    std::cout << "Enter the weight of your carriage (.0 kg): ";
    std::getline(std::cin, weight);

    auto order = new Order("123," + std::to_string(date) + ",2," + "8924 HP-3," + destination + "," + weight + ",100.0");            // check the algorithm of adding new order?
    if (OrdersDAO::insert(*user, *order)) {
        std::cout << "Order was successfully added! \n" << std::endl;
    } else {
        std::cout << "Something went wrong.. \n" << std::endl;
        return;
    }
}

int main() {
    int choice;

    do {
        showMainMenu();
        std::cout << "Choose an option (1-3): ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                login();
                break;
            case 2:
                newOrder();
                break;
            case 3:
                std::cout << "Goodbye!" << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    } while (choice != 3);

    return 0;
}

