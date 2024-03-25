
#ifndef CAR_PARK_PROJECT_CAR_H
#define CAR_PARK_PROJECT_CAR_H
#include <string>

class Car {
private:
    std::string number;
    std::string brand;
    double initial_mileage;
    double capacity;
    double total_mileage;
    double total_weight;
public:
    Car();
    Car(std::string sql_data);
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


#endif //CAR_PARK_PROJECT_CAR_H
