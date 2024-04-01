
#ifndef CAR_PARK_PROJECT_CAR_H
#define CAR_PARK_PROJECT_CAR_H
#include <string>
#include "car_park/user.hpp"

namespace car_park {
class Car {
private:
    std::string number;
    std::string brand;
    double initial_mileage;
    double capacity;
    double total_mileage;
    double total_weight;

    void count_totals();

public:
    Car();
    explicit Car(std::string sql_data);
    double getCapacity(){
        return capacity;
    }
    std::string getNumber(){
        return number;
    }
    std::string getBrand(){
        return brand;
    }
    double getInitialMileage(){
        return initial_mileage;
    }
    double getTotalMileage(){
        return total_mileage;
    }
    double getTotalWeight(){
        return total_weight;
    }
};

class CarsDAO {
public:
    static bool insert(Car& car);
    static Car* find_by_number(User& user, std::string car_number);
    static Car* find_with_max_total_mileage(User& user);
};
}


#endif //CAR_PARK_PROJECT_CAR_H
