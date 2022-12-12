#include <iostream>
#include <utility>
#include <vector>
#include <list>
#include <algorithm>
#include <cassert>
#include "Car.h"
using namespace std;

/*
 * Create a Bus class. This class should inherit from the Car class.
 * In addition, a Bus should have three additional variables:
 *     - a maximum number of passengers,
 *     - a vector of the stop numbers that passengers are getting off at,
 *     - and a gas mileage penalty per passenger.
 * Each element in the vector represents a single passenger getting off at
 * that stop.
 * The Bus class should implement a `generate_route` method that takes as
 * input a starting stop, and returns an std::list of stops to go to.
 *     The list of stops should be all of the unique elements in the
 *     passengers variable sorted in ascending order, but starting with
 *     starting stop and wrapping around to smaller stop numbers.
 * The `move_to` method should be overridden to use extra gas based on the
 * product of the current number of passengers and the mileage per
 * passenger penalty.
 */


class Bus: public Car {
public:
    uint16_t max_passengers;
    vector<uint16_t>* stops;
    double mpg_penalty;

    Bus(double x, double y, double mpg, double tank_size, double fuel,
        uint16_t max_passengers, vector<uint16_t>* stops, double mpg_penalty)
        : Car(x, y, mpg, tank_size, fuel), max_passengers(max_passengers), stops(stops), mpg_penalty(mpg_penalty) {}

    Bus(): Bus(0, 0, 0, 0, 0, 0, new vector<uint16_t>, 0) {}

    [[nodiscard]] list<uint16_t> generate_route(uint16_t start) const {
        auto res = new list<uint16_t>(stops->begin(), stops->end());
        res->sort();
        res->unique();

        while(res->front() != start) {
            res->push_back(res->front());
            res->pop_front();
        }

        return *res;
    }

    bool move_to(double x, double y) override {
        auto extra_gas_usage = mpg_penalty * (double)stops->size();
        mpg -= extra_gas_usage;
        auto res = Car::move_to(x, y);
        if(!res) mpg += extra_gas_usage;
        return res;
    }
};

/*
 * Create a MedicalCenter class. This class should contain a data structure
 * that lists the providers at the center and their roles, as well as another
 * data structure that lists the patients at that center and their condition.
 * Both roles and conditions should be represented with strings.
 */

class MedicalCenter {
public:
    // If I were to want to add more fields to providers or patients
    // in the future I'd use a struct, but for now I'll just use a pair
    vector<pair<string, string>>* providers;
    vector<pair<string, string>>* patients;

    MedicalCenter(): providers(new vector<pair<string, string>>), patients(new vector<pair<string, string>>) {}
    MedicalCenter(vector<pair<string, string>>* providers, vector<pair<string, string>>* patients)
        : providers(providers), patients(patients) {}
};

/*
 * Create an Ambulance class. This class should inherit functionality from
 * both a Car and a MedicalCenter. In addition, an ambulance should have
 * three additional variables; a maximum capacity of patients, a maximum
 * total capacity of patients and providers, and a gas mileage penalty per
 * person. The `move_to` method should be overridden to use extra gas based
 * on the product of the current capacity of the ambulance and the mileage
 * per person penalty.
 */

class Ambulance: public Car, public MedicalCenter {
public:
    uint16_t max_patient_capacity;
    uint16_t max_total_capacity;
    double mpg_penalty;

    Ambulance(double x, double y, double mpg, double tank_size, double fuel,
        vector<pair<string, string>>* providers, vector<pair<string, string>>* patients,
        uint16_t max_patient_capacity, uint16_t max_total_capacity, double mpg_penalty)
        : Car(x, y, mpg, tank_size, fuel), MedicalCenter(providers, patients),
        max_patient_capacity(max_patient_capacity), max_total_capacity(max_total_capacity), mpg_penalty(mpg_penalty) {}

    Ambulance(): Ambulance(0, 0, 0, 0, 0, new vector<pair<string, string>>, new vector<pair<string, string>>, 0, 0, 0) {}

    bool move_to(double x, double y) override {
        auto extra_gas_usage = mpg_penalty * (double)(providers->size() + patients->size());
        mpg -= extra_gas_usage;
        auto res = Car::move_to(x, y);
        if(!res) mpg += extra_gas_usage;
        return res;
    }
};

/*
 * Write a function that takes as input a vector of unique pointers to Cars
 * and a point in two-dimensional space. The function should return a vector
 * containing a copy of each of the cars that can move to that space. The
 * input array and the Cars in it should not be modified. The Cars in the
 * output array should have the `move_to` method called on them prior to
 * returning. Your function should be able to handle instances of Cars,
 * Buses, and Ambulances in the input vector. Each should use the proper
 * `move_to` method.
 */

vector<Car*> move_cars(const vector<Car*>& cars, double x, double y) {
    auto res = new vector<Car*>();
    for(auto car : cars) {
        auto copy = new Car(*car);
        if(copy->move_to(x, y)) res->push_back(copy);
        else delete copy;
    }
    return *res;
}

int main() {
    // Test Bus class
    auto bus = Bus(0, 0, 20, 20, 20, 10, new vector<uint16_t>{1, 2, 3, 4, 5, 6}, 0.2);

    assert((bus.generate_route(1) == list<uint16_t>{1, 2, 3, 4, 5, 6}));
    assert((bus.generate_route(2) == list<uint16_t>{2, 3, 4, 5, 6, 1}));
    assert(bus.move_to(10, 20));

    // Test MedicalCenter class
    auto mc = MedicalCenter(
            new vector<pair<string, string>>{{"Dr. Smith", "Surgeon"}, {"Dr. Jones", "GP"}},
            new vector<pair<string, string>>{{"Jane Doe", "Broken leg"}, {"John Smith", "Flu"}}
    );

    assert((mc.providers->at(0) == pair<string, string>{"Dr. Smith", "Surgeon"}));
    assert((mc.patients->at(1) == pair<string, string>{"John Smith", "Flu"}));

    // Test Ambulance class
    auto ambulance = Ambulance(0, 0, 20, 20, 20, mc.providers, mc.patients, 4, 8, 0.1);

    assert(ambulance.move_to(100, 200));


    return 0;
}
